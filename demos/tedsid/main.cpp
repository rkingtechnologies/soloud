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
#include "soloud_bassboostfilter.h"
#include "soloud_biquadresonantfilter.h"
#include "soloud_dcremovalfilter.h"
#include "soloud_echofilter.h"
#include "soloud_tedsid.h"

namespace {

SoLoud::Soloud gSoloud;
SoLoud::TedSid gMusic1, gMusic2;
SoLoud::BiquadResonantFilter gBiquad;
SoLoud::EchoFilter gEcho;
SoLoud::DCRemovalFilter gDCRemoval;
SoLoud::BassboostFilter gBassboost;
int gMusichandle1, gMusichandle2;

float filter_param0[4] = {0, 0, 0, 0};
float filter_param1[4] = {1000, 2, 0, 0};
float filter_param2[4] = {2, 0, 0, 0};

float song1volume = 1;
float song2volume = 0;

void SetFilters() {
  gSoloud.setFilterParameter(0, 0, 0, filter_param0[0]);
  gSoloud.setFilterParameter(0, 0, 2, filter_param1[0]);
  gSoloud.setFilterParameter(0, 0, 3, filter_param2[0]);

  gSoloud.setFilterParameter(0, 1, 0, filter_param0[1]);
  gSoloud.setFilterParameter(0, 1, 1, filter_param1[1]);

  gSoloud.setFilterParameter(0, 2, 0, filter_param0[2]);

  gSoloud.setFilterParameter(0, 3, 0, filter_param0[3]);
}

void RenderTedsid() {
  SetFilters();

  float* buf = gSoloud.getWave();
  float* fft = gSoloud.calcFFT();

  ImGui::SeparatorText("Output");
  ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
  ImGui::PlotHistogram(
    "##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
  float sidregs[32 + 5];
  int i;
  for (i = 0; i < 32; i++) {
    sidregs[i] = gSoloud.getInfo(gMusichandle1, i);
  }
  for (i = 0; i < 5; i++) {
    sidregs[32 + i] = gSoloud.getInfo(gMusichandle2, i + 64);
  }
  ImGui::PlotHistogram("##SID", sidregs, 32 + 5, 0,
    "          SID               TED", 0, 0xff, ImVec2(264, 80), 4);

  ImGui::Text("SID: %02X %02X %02X %02X %02X %02X %02X %02X",
    (int)gSoloud.getInfo(gMusichandle1, 0),
    (int)gSoloud.getInfo(gMusichandle1, 1),
    (int)gSoloud.getInfo(gMusichandle1, 2),
    (int)gSoloud.getInfo(gMusichandle1, 3),
    (int)gSoloud.getInfo(gMusichandle1, 4),
    (int)gSoloud.getInfo(gMusichandle1, 5),
    (int)gSoloud.getInfo(gMusichandle1, 6),
    (int)gSoloud.getInfo(gMusichandle1, 7));
  ImGui::Text("     %02X %02X %02X %02X %02X %02X %02X %02X",
    (int)gSoloud.getInfo(gMusichandle1, 8),
    (int)gSoloud.getInfo(gMusichandle1, 9),
    (int)gSoloud.getInfo(gMusichandle1, 10),
    (int)gSoloud.getInfo(gMusichandle1, 11),
    (int)gSoloud.getInfo(gMusichandle1, 12),
    (int)gSoloud.getInfo(gMusichandle1, 13),
    (int)gSoloud.getInfo(gMusichandle1, 14),
    (int)gSoloud.getInfo(gMusichandle1, 15));
  ImGui::Text("     %02X %02X %02X %02X %02X %02X %02X %02X",
    (int)gSoloud.getInfo(gMusichandle1, 16),
    (int)gSoloud.getInfo(gMusichandle1, 17),
    (int)gSoloud.getInfo(gMusichandle1, 18),
    (int)gSoloud.getInfo(gMusichandle1, 19),
    (int)gSoloud.getInfo(gMusichandle1, 20),
    (int)gSoloud.getInfo(gMusichandle1, 21),
    (int)gSoloud.getInfo(gMusichandle1, 22),
    (int)gSoloud.getInfo(gMusichandle1, 23));
  ImGui::Text("     %02X %02X %02X %02X %02X %02X %02X %02X",
    (int)gSoloud.getInfo(gMusichandle1, 24),
    (int)gSoloud.getInfo(gMusichandle1, 25),
    (int)gSoloud.getInfo(gMusichandle1, 26),
    (int)gSoloud.getInfo(gMusichandle1, 27),
    (int)gSoloud.getInfo(gMusichandle1, 28),
    (int)gSoloud.getInfo(gMusichandle1, 29),
    (int)gSoloud.getInfo(gMusichandle1, 30),
    (int)gSoloud.getInfo(gMusichandle1, 31));
  ImGui::Text("TED: %02X %02X %02X %02X %02X",
    (int)gSoloud.getInfo(gMusichandle2, 64),
    (int)gSoloud.getInfo(gMusichandle2, 65),
    (int)gSoloud.getInfo(gMusichandle2, 66),
    (int)gSoloud.getInfo(gMusichandle2, 67),
    (int)gSoloud.getInfo(gMusichandle2, 68));

  ImGui::SeparatorText("Control");
  ImGui::Text("Song volumes");

  if (ImGui::SliderFloat("Song1 vol", &song1volume, 0, 1)) {
    gSoloud.setVolume(gMusichandle1, song1volume);
  }
  if (ImGui::SliderFloat("Song2 vol", &song2volume, 0, 1)) {
    gSoloud.setVolume(gMusichandle2, song2volume);
  }
  ImGui::Separator();
  ImGui::Text("Biquad filter (lowpass)");
  ImGui::SliderFloat("Wet##4", &filter_param0[0], 0, 1);
  ImGui::SliderFloat("Frequency##4", &filter_param1[0], 0, 8000);
  ImGui::SliderFloat("Resonance##4", &filter_param2[0], 1, 20);
  ImGui::Separator();
  ImGui::Text("Bassboost filter");
  ImGui::SliderFloat("Wet##2", &filter_param0[1], 0, 1);
  ImGui::SliderFloat("Boost##2", &filter_param1[1], 0, 11);
  ImGui::Separator();
  ImGui::Text("Echo filter");
  ImGui::SliderFloat("Wet##3", &filter_param0[2], 0, 1);
  ImGui::Separator();
  ImGui::Text("DC removal filter");
  ImGui::SliderFloat("Wet##1", &filter_param0[3], 0, 1);
}

class Content : public soloud::tests::common::Renderable {
 public:
  void Render(soloud::tests::common::Window* window) override {
    static bool running = true;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGui::Begin("##SpeechFilterDemo", &running,
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_AlwaysAutoResize);

    if (!running) {
      window->Close();
    }

    RenderTedsid();

    ImGui::End();
  }
};

void LoadAudioFile(const std::filesystem::path& path, SoLoud::TedSid& tedsid) {
  if (!std::filesystem::exists(path)) {
    std::println("Could not find audio file: {}", path.string());
    return;
  }

  tedsid.load(path.string().c_str());
}

void LoadAudioFiles() {
  const auto asset_dir = demo_asset_manager::FindDemoAssetsDir();

  if (asset_dir == std::nullopt) {
    std::println("Could not find demo assets directory!");
    return;  // prevent against dereferencing nullopt
  }

  LoadAudioFile(*asset_dir / "audio" / "Modulation.sid.dump", gMusic1);
  LoadAudioFile(*asset_dir / "audio" / "ted_storm.prg.dump", gMusic2);
}

void InitAudio() {
  LoadAudioFiles();

  gEcho.setParams(0.2f, 0.5f, 0.05f);
  gBiquad.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 4000, 2);

  gMusic1.setLooping(1);
  gMusic2.setLooping(1);

  gSoloud.setGlobalFilter(0, &gBiquad);
  gSoloud.setGlobalFilter(1, &gBassboost);
  gSoloud.setGlobalFilter(2, &gEcho);
  gSoloud.setGlobalFilter(3, &gDCRemoval);

  gSoloud.init(
    SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);

  gMusichandle1 = gSoloud.play(gMusic1);
  gMusichandle2 = gSoloud.play(gMusic2, 0);
}

}  // namespace

int main() {
  InitAudio();

  SetFilters();

  soloud::tests::common::Window window({"Tedsid Demo", 640, 720});
  window.AddRenderable<Content>();
  window.Run();

  return 0;
}