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

#include <print>

#include "common/asset_manager.h"
#include "common/window.h"
#include "imgui.h"
#include "soloud.h"
#include "soloud_biquadresonantfilter.h"
#include "soloud_wav.h"

namespace {

SoLoud::Soloud g_soloud;
SoLoud::BiquadResonantFilter lp_filter;
SoLoud::Wav rain, wind, music;

int rain_handle, wind_handle, music_handle;

float UpdateAudio() {
  const int tick = static_cast<int>(ImGui::GetTime() * 1000);
  const float p = (tick % 60000) / 60000.0f;

  static int mode_a = 0;

  if (p < 0.35f) {
    if (mode_a != 0) {
      g_soloud.fadeVolume(rain_handle, 1, 0.2f);
    }
    mode_a = 0;
  } else {
    if (mode_a != 1) {
      g_soloud.fadeVolume(rain_handle, 0, 0.2f);
    }
    mode_a = 1;
  }

  static int mode_b = 0;

  if (p < 0.7f) {
    if (mode_b != 0) {
      g_soloud.fadeVolume(wind_handle, 0, 0.2f);
    }
    mode_b = 0;
  } else if (p < 0.8f) {
    g_soloud.setVolume(wind_handle, (p - 0.7f) * 10);
    mode_b = 1;
  } else {
    if (mode_b != 2) {
      g_soloud.fadeVolume(wind_handle, 1, 0.2f);
    }
    mode_b = 2;
  }

  static int mode_c = 0;

  if (p < 0.2f) {
    if (mode_c != 0) {
      g_soloud.fadeVolume(music_handle, 0, 0.2f);
    }
    mode_c = 0;
  } else if (p < 0.4f) {
    g_soloud.setVolume(music_handle, (p - 0.2f) * 5);
    mode_c = 1;
  } else if (p < 0.5f) {
    if (mode_c != 2) {
      g_soloud.fadeVolume(music_handle, 1, 0.2f);
    }
    mode_c = 2;
  } else if (p < 0.7f) {
    g_soloud.setVolume(music_handle, 1 - (p - 0.5f) * 4.5f);
    mode_c = 3;
  } else {
    if (mode_c != 4) {
      g_soloud.fadeVolume(music_handle, 0.1f, 0.2f);
    }
    mode_c = 4;
  }

  static int mode_d = 0;

  if (p < 0.25f) {
    if (mode_d != 0) {
      g_soloud.fadeFilterParameter(
        music_handle, 0, SoLoud::BiquadResonantFilter::FREQUENCY, 200, 0.2f);
      g_soloud.fadeFilterParameter(
        music_handle, 0, SoLoud::BiquadResonantFilter::WET, 1, 0.2f);
    }
    mode_d = 0;
  } else if (p < 0.35f) {
    if (mode_d != 1) {
      g_soloud.fadeFilterParameter(
        music_handle, 0, SoLoud::BiquadResonantFilter::WET, 0.5f, 2.0f);
    }
    mode_d = 1;
  } else if (p < 0.55f) {
    if (mode_d != 2) {
      g_soloud.fadeFilterParameter(
        music_handle, 0, SoLoud::BiquadResonantFilter::FREQUENCY, 2000, 1.0f);
      g_soloud.fadeFilterParameter(
        music_handle, 0, SoLoud::BiquadResonantFilter::WET, 0, 1.0f);
    }
    mode_d = 2;
  } else {
    if (mode_d != 3) {
      g_soloud.fadeFilterParameter(
        music_handle, 0, SoLoud::BiquadResonantFilter::FREQUENCY, 200, 0.3f);
      g_soloud.fadeFilterParameter(
        music_handle, 0, SoLoud::BiquadResonantFilter::WET, 1, 0.3f);
    }
    mode_d = 3;
  }

  static int mode_e = 0;

  if (p < 0.2f) {
    if (mode_e != 0) {
      g_soloud.fadePan(music_handle, 1, 0.2f);
    }
    mode_e = 0;
  } else if (p < 0.4f) {
    g_soloud.setPan(music_handle, 1 - ((p - 0.2f) * 5));
    mode_e = 1;
  } else {
    if (mode_e != 2) {
      g_soloud.fadePan(music_handle, 0, 0.2f);
    }
    mode_e = 2;
  }
  return p;
}

const char* GetDescription(float p) {
  if (p < 0.2f) {
    return "Outside, heavy rain pouring";
  }

  if (p < 0.35f) {
    return "Approaching the club entrance - rain still falling";
  }

  if (p < 0.4f) {
    return "Walking through the club door - music starts fading in";
  }

  if (p < 0.5f) {
    return "Inside the club - full music, lively atmosphere";
  }

  if (p < 0.55f) {
    return "Entering the elevator - doors closing, music muffled";
  }

  if (p < 0.7f) {
    return "Elevator ascending - music distant, wind outside";
  }

  return "On the rooftop - windy night, quiet music";
}

void RenderSoloud() {
  float p = UpdateAudio();
  const char* const description = GetDescription(p);

  float* buf = g_soloud.getWave();
  float* fft = g_soloud.calcFFT();

  ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
  ImGui::PlotHistogram(
    "##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
  ImGui::Text("Active voices: %d", g_soloud.getActiveVoiceCount());

  ImGui::Text("Progress: %3.3f%%", 100 * p);
  ImGui::Text("Rain volume: %3.3f", g_soloud.getVolume(rain_handle));
  ImGui::Text("Music volume: %3.3f", g_soloud.getVolume(music_handle));
  ImGui::Text("Wind volume: %3.3f", g_soloud.getVolume(wind_handle));
  ImGui::Text("Music pan: %3.3f", g_soloud.getPan(music_handle));
  ImGui::Text(
    "Music filter wet: %3.3f", g_soloud.getFilterParameter(music_handle, 0,
                                 SoLoud::BiquadResonantFilter::WET));
  ImGui::Text(
    "Music filter freq: %3.3f", g_soloud.getFilterParameter(music_handle, 0,
                                  SoLoud::BiquadResonantFilter::FREQUENCY));
  ImGui::Text("Description: %s", description);
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

    ImGui::Begin("##Environmental Sound Demo", &running, window_flags);

    if (!running) {
      window->Close();
    }

    RenderSoloud();

    ImGuiIO& io = ImGui::GetIO();

    ImGui::Separator();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
      1000.0f / io.Framerate, io.Framerate);

    ImGui::End();
  }
};

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

  LoadAudioFile(*asset_dir / "audio" / "thunder_and_rain.wav", rain);
  LoadAudioFile(*asset_dir / "audio" / "windy_ambience.ogg", wind);
  LoadAudioFile(*asset_dir / "audio" / "tetsno.ogg", music);
}

void InitAudio() {
  g_soloud.init(
    SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
  g_soloud.setGlobalVolume(0.75);
  g_soloud.setPostClipScaler(0.75);

  LoadAudioFiles();

  rain.setLooping(1);
  wind.setLooping(1);
  music.setLooping(1);
  
  lp_filter.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 100, 10);
  music.setFilter(0, &lp_filter);

  rain_handle = g_soloud.play(rain, 1);
  wind_handle = g_soloud.play(wind, 0);
  music_handle = g_soloud.play(music, 0);
}

}  // namespace

int main() {
  InitAudio();

  soloud::tests::common::Window window({"Environmental Sound Demo", 640, 640});
  window.AddRenderable<Content>();
  window.Run();
}