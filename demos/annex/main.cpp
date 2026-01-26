/*
SoLoud audio engine
Copyright (c) 2013-2020 Jari Komppa

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
#include "common/simple_window.h"
#include "common/window.h"
#include "imgui.h"
#include "soloud.h"
#include "soloud_biquadresonantfilter.h"
#include "soloud_echofilter.h"
#include "soloud_freeverbfilter.h"
#include "soloud_lofifilter.h"
#include "soloud_wavstream.h"

namespace {

SoLoud::Soloud gSoloud;
SoLoud::WavStream gMusic;
SoLoud::Bus gBus1, gBus2, gBus3, gBus4;
SoLoud::LofiFilter gLofi;
SoLoud::BiquadResonantFilter gBiquad;
SoLoud::EchoFilter gEcho;
SoLoud::FreeverbFilter gVerb;

int gMusichandle;
int gBus1handle;
int gFrozen;

void RenderAnnex() {
  float* buf = gSoloud.getWave();
  float* fft = gSoloud.calcFFT();

  ImGui::SeparatorText("Output");
  ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
  ImGui::PlotHistogram(
    "##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
  ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
  ImGui::Text("Bus 1 voices     : %d", gBus1.getActiveVoiceCount());
  ImGui::Text("Bus 2 voices     : %d", gBus2.getActiveVoiceCount());
  ImGui::Text("Bus 3 voices     : %d", gBus3.getActiveVoiceCount());
  ImGui::Text("Bus 4 voices     : %d", gBus4.getActiveVoiceCount());

  ImGui::SeparatorText("Control");
  if (ImGui::Button("Annex sound to bus 1")) {
    gBus1.annexSound(gMusichandle);
  }
  ImGui::SameLine();
  if (!gFrozen && ImGui::Button("Freeze")) {
    gSoloud.setFilterParameter(
      gBus1handle, 0, SoLoud::FreeverbFilter::FREEZE, 1);
    gFrozen = 1;
  }
  if (gFrozen && ImGui::Button("Thaw")) {
    gSoloud.setFilterParameter(
      gBus1handle, 0, SoLoud::FreeverbFilter::FREEZE, 0);
    gFrozen = 0;
  }

  if (ImGui::Button("Annex sound to bus 2")) {
    gBus2.annexSound(gMusichandle);
  }
  if (ImGui::Button("Annex sound to bus 3")) {
    gBus3.annexSound(gMusichandle);
  }
  if (ImGui::Button("Annex sound to bus 4")) {
    gBus4.annexSound(gMusichandle);
  }
}

void LoadMusic() {
  const auto asset_dir = demo_asset_manager::FindDemoAssetsDir();

  if (asset_dir == std::nullopt) {
    std::println("Could not find demo assets directory!");
    return;  // prevent against dereferencing nullopt
  }

  const std::filesystem::path& music_path =
    *asset_dir / "audio" / "delphi_loop.ogg";

  if (!std::filesystem::exists(music_path)) {
    std::println("Could not find music file: {}", music_path.string());
    return;
  }

  gMusic.load(music_path.string().c_str());
}

void InitAudio() {
  gSoloud.init(
    SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);

  gFrozen = 0;

  LoadMusic();
  gMusic.setLooping(1);

  gLofi.setParams(1000, 6);
  gBiquad.setParams(SoLoud::BiquadResonantFilter::HIGHPASS, 500, 2);
  gEcho.setParams(0.25f, 0.9f);

  gBus2.setFilter(0, &gLofi);
  gBus3.setFilter(0, &gBiquad);
  gBus4.setFilter(0, &gEcho);
  gBus1.setFilter(0, &gVerb);

  gBus1handle = gSoloud.play(gBus1);
  gSoloud.play(gBus2);
  gSoloud.play(gBus3);
  gSoloud.play(gBus4);
  gMusichandle = gBus1.play(gMusic);
}

}  // namespace

int main() {
  InitAudio();

  SimpleWindow simple_window(RenderAnnex);
  soloud::tests::common::Window window({"Annex Demo", 400, 600});
  window.AddRenderable<SimpleWindow>(simple_window);
  window.Run();
}