/*
SoLoud audio engine
Copyright (c) 2013-2015 Jari Komppa

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <print>

#include "common/asset_manager.h"
#include "common/window.h"
#include "imgui.h"
#include "soloud.h"
#include "soloud_speech.h"
#include "soloud_wav.h"

namespace {

SoLoud::Soloud gSoloud;  // SoLoud engine core
SoLoud::Speech gSpeech[10];
SoLoud::Wav gSfxloop, gMusicloop;
SoLoud::Bus gSfxbus, gMusicbus, gSpeechbus;

int gSfxbusHandle, gMusicbusHandle, gSpeechbusHandle;
float gSfxvol = 1, gMusicvol = 1, gSpeechvol = 1;

int speechtick = 0;
int speechcount = 0;

void RenderMixBusses() {
  if (speechtick < ImGui::GetTime() * 1000) {
    int h = gSpeechbus.play(gSpeech[speechcount % 10],
      (rand() % 200) / 50.0f + 2, (rand() % 20) / 10.0f - 1);
    speechcount++;
    gSoloud.setRelativePlaySpeed(h, (rand() % 100) / 200.0f + 0.75f);
    gSoloud.fadePan(h, (rand() % 20) / 10.0f - 1, 2);
    speechtick = (ImGui::GetTime() * 1000) + 4000;
  }

  float* buf = gSoloud.getWave();
  float* fft = gSoloud.calcFFT();

  ImGui::SeparatorText("Output");
  ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
  ImGui::PlotHistogram(
    "##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
  ImGui::Text("Speech bus volume : %d%%",
    (int)floor(gSoloud.getVolume(gSpeechbusHandle) * 100));
  ImGui::Text("Music bus volume  : %d%%",
    (int)floor(gSoloud.getVolume(gMusicbusHandle) * 100));
  ImGui::Text("Sfx bus volume    : %d%%",
    (int)floor(gSoloud.getVolume(gSfxbusHandle) * 100));
  ImGui::Text("Active voices     : %d", gSoloud.getActiveVoiceCount());

  ImGui::SeparatorText("Control");
  if (ImGui::SliderFloat("Speech bus volume", &gSpeechvol, 0, 2)) {
    gSoloud.setVolume(gSpeechbusHandle, gSpeechvol);
  }
  if (ImGui::SliderFloat("Music bus volume", &gMusicvol, 0, 2)) {
    gSoloud.setVolume(gMusicbusHandle, gMusicvol);
  }
  if (ImGui::SliderFloat("Sfx bus volume", &gSfxvol, 0, 2)) {
    gSoloud.setVolume(gSfxbusHandle, gSfxvol);
  }
}

class Content : public soloud::tests::common::Renderable {
  void Render(soloud::tests::common::Window* window) override final {
    static bool running = true;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    const static ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
      ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::Begin("##content", &running, window_flags);

    if (!running) {
      window->Close();
    }

    RenderMixBusses();

    ImGui::End();
  }
};

void SetupSpeech() {
  gSpeech[0].setText("There is flaky pastry in my volkswagon.");
  gSpeech[1].setText(
    "The fragmentation of empiricism is hardly influential in its "
    "interdependence.");
  gSpeech[2].setText("Sorry, my albatros is not inflatable.");
  gSpeech[3].setText(
    "The clairvoyance of omnipotence is in fact quite closed-minded in its "
    "ecology.");
  gSpeech[4].setText("Cheese is quite nice.");
  gSpeech[5].setText("Pineapple Scones with Squash and Pastrami Sandwich");
  gSpeech[6].setText(
    "The smart trader nowadays will be sure not to prorate OTC special-purpose "
    "entities.");
  gSpeech[7].setText("The penguins are in the toilets.");
  gSpeech[8].setText(
    "Don't look, but there is a mountain lion stalking your children");
  gSpeech[9].setText(
    "The train has already gone, would you like to hire a bicycle?");
}

void LoadAudioFile(const std::filesystem::path& path, SoLoud::Wav& wav) {
  if (!std::filesystem::exists(path)) {
    std::println("Could not find audio file: {}", path.string());
    return;
  }

  wav.load(path.string().c_str());
}

void LoadAudioFiles() {
  const auto asset_dir = demo_asset_manager::FindDemoAssetsDir();

  if (asset_dir == std::nullopt) {
    std::println("Could not find demo assets directory!");
    return;  // prevent against dereferencing nullopt
  }

  LoadAudioFile(*asset_dir / "audio" / "war_loop.ogg", gSfxloop);
  LoadAudioFile(*asset_dir / "audio" / "algebra_loop.ogg", gMusicloop);
}

void InitAudio() {
  gSoloud.init(
    SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
  gSoloud.setGlobalVolume(0.75);
  gSoloud.setPostClipScaler(0.75);

  gSpeechbusHandle = gSoloud.play(gSpeechbus);
  gSfxbusHandle = gSoloud.play(gSfxbus);
  gMusicbusHandle = gSoloud.play(gMusicbus);

  SetupSpeech();

  LoadAudioFiles();

  gSfxloop.setLooping(1);
  gMusicloop.setLooping(1);

  gSfxbus.play(gSfxloop);
  gMusicbus.play(gMusicloop);
}

}  // namespace

int main() {
  InitAudio();

  soloud::tests::common::Window window({"Mixbusses Demo", 650, 550});
  window.AddRenderable<Content>();
  window.Run();
}