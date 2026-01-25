/*
SoLoud audio engine
Copyright (c) 2013-2018 Jari Komppa

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

#include "common/window.h"
#include "imgui.h"
#include "soloud.h"
#include "soloud_sfxr.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"

namespace {

const char* filenames[] = {"ch1.flac", "ch1.mp3", "ch1.ogg", "ch1_16bit.wav",
  "ch1_24bit.wav", "ch1_32bit.wav", "ch1_8bit.wav", "ch1_alaw.wav",
  "ch1_double.wav", "ch1_float.wav", "ch1_imaadpcm.wav", "ch1_msadpcm.wav",
  "ch1_ulaw.wav", "ch2.flac", "ch2.mp3", "ch2.ogg", "ch2_16bit.wav",
  "ch2_24bit.wav", "ch2_32bit.wav", "ch2_8bit.wav", "ch2_alaw.wav",
  "ch2_double.wav", "ch2_float.wav", "ch2_imaadpcm.wav", "ch2_msadpcm.wav",
  "ch2_ulaw.wav", "ch4.flac", "ch4.ogg", "ch4_16bit.wav", "ch4_24bit.wav",
  "ch4_32bit.wav", "ch4_8bit.wav", "ch4_alaw.wav", "ch4_double.wav",
  "ch4_float.wav", "ch4_imaadpcm.wav", "ch4_msadpcm.wav", "ch4_ulaw.wav",
  "ch6.flac", "ch6.ogg", "ch6_16bit.wav", "ch6_24bit.wav", "ch6_32bit.wav",
  "ch6_8bit.wav", "ch6_alaw.wav", "ch6_double.wav", "ch6_float.wav",
  "ch6_imaadpcm.wav", "ch6_msadpcm.wav", "ch6_ulaw.wav", "ch8.flac", "ch8.ogg",
  "ch8_16bit.wav", "ch8_24bit.wav", "ch8_32bit.wav", "ch8_8bit.wav",
  "ch8_alaw.wav", "ch8_double.wav", "ch8_float.wav", "ch8_imaadpcm.wav",
  "ch8_msadpcm.wav", "ch8_ulaw.wav"};

int files = sizeof(filenames) / sizeof(char*);
SoLoud::Soloud gSoloud;
SoLoud::Wav* gWav;
bool* gWavOk;
SoLoud::WavStream* gWavstream;
bool* gWavStreamOk;

void InitAudio() {
  gSoloud.init(
    SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);

  gWav = new SoLoud::Wav[files];
  gWavstream = new SoLoud::WavStream[files];
  gWavOk = new bool[files];
  gWavStreamOk = new bool[files];

  int i;
  for (i = 0; i < files; i++) {
    char temp[256];
    sprintf(temp, "../assets/audio/wavformats/%s", filenames[i]);
    gWavOk[i] = gWav[i].load(temp) == SoLoud::SO_NO_ERROR;
    gWavStreamOk[i] = gWavstream[i].load(temp) == SoLoud::SO_NO_ERROR;
  }
}

void RenderOutput() {
  float* buf = gSoloud.getWave();
  float* fft = gSoloud.calcFFT();

  ImGui::SeparatorText("Output");
  ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
  ImGui::PlotHistogram(
    "##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
  ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
}

void RenderControls() {
  ImGui::SeparatorText("Control");

  static int item_selected_idx = 0;
  const char* combo_preview_value = filenames[item_selected_idx];

  if (ImGui::BeginCombo("Select Wav Format", combo_preview_value)) {
    static ImGuiTextFilter filter;
    if (ImGui::IsWindowAppearing()) {
      ImGui::SetKeyboardFocusHere();
      filter.Clear();
    }
    filter.Draw("##Filter", -FLT_MIN);

    for (int n = 0; n < files; n++) {
      const bool is_selected = (item_selected_idx == n);
      if (filter.PassFilter(filenames[n])) {
        if (ImGui::Selectable(filenames[n], is_selected)) {
          item_selected_idx = n;
        }
      }
    }
    ImGui::EndCombo();
  }

  ImGui::SameLine();

  // 1. Trigger the popup logic
  if (ImGui::Button("Play")) {
    if (gWavOk[item_selected_idx]) {
      gSoloud.play(gWav[item_selected_idx]);
    } else {
      ImGui::OpenPopup("Error##Wav");  // Just trigger it here
    }
  }

  // 2. Define the popup outside the Button conditional
  // This must be called every frame to keep the popup alive
  if (ImGui::BeginPopupModal(
        "Error##Wav", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Failed to load wav format:\n%s", filenames[item_selected_idx]);
    ImGui::Separator();

    if (ImGui::Button("OK", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void RenderWavFormats() {
  RenderOutput();
  RenderControls();
}

class Content : public soloud::tests::common::Renderable {
 public:
  void Render(soloud::tests::common::Window* window) override {
    static bool running = true;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGui::Begin("##Wavformats", &running,
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_AlwaysAutoResize);

    if (!running) {
      window->Close();
    }

    RenderWavFormats();

    ImGui::End();
  }
};

}  // namespace

int main() {
  InitAudio();

  soloud::tests::common::Window window({"Wavformats Demo", 640, 640});
  window.AddRenderable<Content>();
  window.Run();
}