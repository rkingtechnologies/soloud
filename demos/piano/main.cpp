/*
SoLoud audio engine
Copyright (c) 2013-2021 Jari Komppa

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
#include <algorithm>
#include <array>
#include <format>
#include <memory>
#include <print>
#include <ranges>
#include <string>

#include "common/asset_manager.h"
#include "common/window.h"
#include "imgui.h"
#include "rt_midi.h"
#include "soloud.h"
#include "soloud_ay.h"
#include "soloud_basicwave.h"
#include "soloud_bassboostfilter.h"
#include "soloud_biquadresonantfilter.h"
#include "soloud_dcremovalfilter.h"
#include "soloud_echofilter.h"
#include "soloud_eqfilter.h"
#include "soloud_fftfilter.h"
#include "soloud_flangerfilter.h"
#include "soloud_freeverbfilter.h"
#include "soloud_lofifilter.h"
#include "soloud_padsynth.h"
#include "soloud_robotizefilter.h"
#include "soloud_wav.h"
#include "soloud_waveshaperfilter.h"

namespace {

// Soloud
SoLoud::Soloud g_soloud;
SoLoud::Bus g_bus;
int g_bus_handle;

// Synth Engine Selection
enum class SynthEngine { kGeneratedWave, kPadsynth, kFileSample, kSuperwave };
SynthEngine g_synth_engine = SynthEngine::kGeneratedWave;

// Padsynth
constexpr int kPadsynthAharmonics = 7;
SoLoud::Wav g_padsynth;
std::array<float, kPadsynthAharmonics> g_harmonics = {
  0.7f, 0.3f, 0.2f, 1.7f, 0.4f, 1.3f, 0.2f};
float g_bw = 0.25f;
float g_bws = 1.0f;

// Filters
std::array<SoLoud::Filter*, 11> g_filter;

// For Playing Notes
struct Plonked {
  int handle;
  float rel;
};
std::array<Plonked, 128> g_plonked;
float g_release = 0.5f;

// Waveform
SoLoud::Basicwave g_wave;

void LoadAudioFile(const std::filesystem::path& path, SoLoud::Wav& wav) {
  if (!std::filesystem::exists(path)) {
    std::println("Could not find audio file: {}", path.string());
    return;
  }

  wav.load(path.string().c_str());
}

void Plonk(float rel, float vol = 0x50) {
  auto it = std::ranges::find_if(
    g_plonked, [](const auto& e) { return e.handle == 0; });

  if (it == g_plonked.end()) {
    return;
  }

  // Adjust vol
  vol = (vol + 10) / (float)(0x7f + 10);
  vol *= vol;

  static SoLoud::Wav sample_wav;

  int handle = 0;
  switch (g_synth_engine) {
    default:
    case SynthEngine::kGeneratedWave:
      g_wave.setFreq(440.0f * rel * 2);
      handle = g_bus.play(g_wave, 0);
      break;
    case SynthEngine::kPadsynth:
      handle = g_bus.play(g_padsynth, 0);
      g_soloud.setRelativePlaySpeed(handle, 2 * rel);
      break;
    case SynthEngine::kFileSample:
      if (static bool first = true; first) {
        const auto asset_dir = demo_asset_manager::FindDemoAssetsDir();

        if (asset_dir == std::nullopt) {
          std::println("Could not find demo assets directory!");
          return;  // prevent against dereferencing nullopt
        }

        LoadAudioFile(*asset_dir / "audio" / "AKWF_c604_0024.wav", sample_wav);

        sample_wav.setLooping(1);
        first = false;
      }
      handle = g_bus.play(sample_wav, 0);
      g_soloud.setRelativePlaySpeed(handle, 2 * rel);
      break;
    case SynthEngine::kSuperwave:
      g_wave.setFreq(440.0f * rel * 2, true);
      handle = g_bus.play(g_wave, 0);
      break;
  }
  constexpr float kAttack = 0.02f;
  g_soloud.fadeVolume(handle, vol, kAttack);
  it->handle = handle;
  it->rel = rel;
}

void Unplonk(float rel) {
  auto it = std::ranges::find_if(
    g_plonked, [rel](const auto& e) { return e.rel == rel; });

  if (it == g_plonked.end()) {
    return;
  }

  g_soloud.fadeVolume(it->handle, 0, g_release);
  g_soloud.scheduleStop(it->handle, g_release);
  it->handle = 0;
}

void RenderSelectWave() {
  static int wave_select = SoLoud::Soloud::WAVE_SQUARE;

  const char* opts =
    "Square wave\x00"
    "Saw wave\x00"
    "Sine wave\x00"
    "Triangle wave\x00"
    "Bounce wave\x00"
    "Jaws wave\x00"
    "Humps wave\x00"
    "Antialized square wave\x00"
    "Antialiazed sawe wave\x00"
    "\x00";

  if (ImGui::Combo("Wave", &wave_select, opts)) {
    g_wave.setWaveform(wave_select);
  }

  ImGui::DragFloat("Attack", &g_wave.mADSR.mA, 0.01f);
  ImGui::DragFloat("Decay", &g_wave.mADSR.mD, 0.01f);
  ImGui::DragFloat("Sustain", &g_wave.mADSR.mS, 0.01f);
  ImGui::DragFloat("Release", &g_release, 0.01f);
}

void RenderWaveform() {
  ImGui::SeparatorText("Waveform");
  RenderSelectWave();
}

void RenderSuperwave() {
  ImGui::SeparatorText("Superwave");
  ImGui::DragFloat("Scale", &g_wave.mSuperwaveScale, 0.01f);
  ImGui::DragFloat("Detune", &g_wave.mSuperwaveDetune, 0.001f);
  RenderSelectWave();
}

void RenderInfoWindow() {
  const float* const buf = g_soloud.getWave();
  const float* const fft = g_soloud.calcFFT();

  ImGui::SeparatorText("Output");
  ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
  ImGui::SameLine();
  ImGui::PlotHistogram(
    "##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
  ImGui::Text("Active voices     : %d", g_soloud.getActiveVoiceCount());
  ImGui::Text("Use your keyboard to play the piano:");
  ImGui::Text("\t1 2 3   5 6   8 9 0");
  ImGui::Text("\tQ W E R T Y U I O P");
}

void RenderPadsynth() {
  ImGui::SeparatorText("Padsynth");

  bool changed = false;

  for (int i = 0; i < kPadsynthAharmonics; ++i) {
    const std::string label = std::format("Harmonic {}", i + 1);
    changed |= ImGui::DragFloat(label.c_str(), &g_harmonics[i], 0.1f);
  }

  changed |= ImGui::DragFloat("Bandwidth", &g_bw, 0.1f);
  changed |= ImGui::DragFloat("Bandwidth scale", &g_bws, 0.1f);

  if (changed) {
    SoLoud::generatePadsynth(
      g_padsynth, kPadsynthAharmonics, g_harmonics.data(), g_bw, g_bws);
  }
}

void RenderFilterWindow() {
  ImGui::SeparatorText("Filter");

  constexpr int kMaxFilters = 4;

  std::array<const char*, kMaxFilters> labels = {
    "Filter 1", "Filter 2", "Filter 3", "Filter 4"};

  const char* filter_opts =
    "None\x00"
    "BassboostFilter\x00"
    "BiquadResonantFilter\x00"
    "DCRemovalFilter\x00"
    "EchoFilter\x00"
    "FFTFilter\x00"
    "FlangerFilter\x00"
    "FreeverbFilter\x00"
    "LofiFilter\x00"
    "RobotizeFilter\x00"
    "WaveShaperFilter\x00"
    "EqFilter\x00\x00";

  static std::array<int, kMaxFilters> filter_selections = {0, 0, 0, 0};

  for (int filter_idx = 0; filter_idx < kMaxFilters; filter_idx++) {
    if (filter_idx != 0) {
      ImGui::Separator();
    }

    int& cur = filter_selections[filter_idx];

    if (ImGui::Combo(labels[filter_idx], &cur, filter_opts)) {
      g_soloud.setGlobalFilter(filter_idx, cur ? g_filter[cur - 1] : 0);
    }

    if (cur == 0) {
      continue;
    }

    SoLoud::Filter* f = g_filter[cur - 1];

    const int count = f->getParamCount();

    for (int i = 0; i < count; ++i) {
      const auto filter_type = f->getParamType(i);

      const float filter_min = f->getParamMin(i);
      const float filter_max = f->getParamMax(i);

      if (filter_type == SoLoud::Filter::INT_PARAM) {
        int v = (int)g_soloud.getFilterParameter(0, filter_idx, i);
        char temp[128];
        sprintf(temp, "%s##%d-%d", f->getParamName(i), filter_idx, i);
        if (ImGui::SliderInt(temp, &v, (int)filter_min, (int)filter_max)) {
          g_soloud.setFilterParameter(0, filter_idx, i, (float)v);
        }
      }

      else if (filter_type == SoLoud::Filter::FLOAT_PARAM) {
        float v = g_soloud.getFilterParameter(0, filter_idx, i);
        char temp[128];
        sprintf(temp, "%s##%d-%d", f->getParamName(i), filter_idx, i);
        if (ImGui::SliderFloat(temp, &v, filter_min, filter_max)) {
          g_soloud.setFilterParameter(0, filter_idx, i, v);
        }
      }

      else if (filter_type == SoLoud::Filter::BOOL_PARAM) {
        float v = g_soloud.getFilterParameter(0, filter_idx, i);
        bool bv = v > 0.5f;
        char temp[128];
        sprintf(temp, "%s##%d-%d", f->getParamName(i), filter_idx, i);
        if (ImGui::Checkbox(temp, &bv)) {
          g_soloud.setFilterParameter(0, filter_idx, i, bv ? 1.0f : 0.0f);
        }
      }
    }
  }
}

template <ImGuiKey key, int p>
void HandleKey() {
  constexpr float magic = 0.943875f;

  if (ImGui::IsKeyPressed(key, false)) {
    Plonk(std::pow(magic, p));
  }

  if (ImGui::IsKeyReleased(key)) {
    Unplonk(std::pow(magic, p));
  }
}

void HandleKeys() {
  HandleKey<ImGuiKey_1, 18>();  // F#
  HandleKey<ImGuiKey_Q, 17>();  // G
  HandleKey<ImGuiKey_2, 16>();  // G#w
  HandleKey<ImGuiKey_W, 15>();  // A
  HandleKey<ImGuiKey_3, 14>();  // A#
  HandleKey<ImGuiKey_E, 13>();  // B
  HandleKey<ImGuiKey_R, 12>();  // C
  HandleKey<ImGuiKey_5, 11>();  // C#
  HandleKey<ImGuiKey_T, 10>();  // D
  HandleKey<ImGuiKey_6, 9>();   // D#
  HandleKey<ImGuiKey_Y, 8>();   // E
  HandleKey<ImGuiKey_U, 7>();   // F
  HandleKey<ImGuiKey_8, 6>();   // F#
  HandleKey<ImGuiKey_I, 5>();   // G
  HandleKey<ImGuiKey_9, 4>();   // G#
  HandleKey<ImGuiKey_O, 3>();   // A
  HandleKey<ImGuiKey_0, 2>();   // A#
  HandleKey<ImGuiKey_P, 1>();   // B
}

void SetSoloudFilterParams() {
  constexpr float filter_param0[4] = {0, 0, 1, 1};  // why arrays?
  constexpr float filter_param1[4] = {8000, 0, 1000, 0};
  constexpr float filter_param2[4] = {3, 0, 2, 0};

  g_soloud.setFilterParameter(g_bus_handle, 0, 0, filter_param0[0]);
  g_soloud.setFilterParameter(g_bus_handle, 1, 0, filter_param0[1]);
  g_soloud.setFilterParameter(g_bus_handle, 2, 0, filter_param0[2]);
  g_soloud.setFilterParameter(g_bus_handle, 3, 0, filter_param0[3]);
  g_soloud.setFilterParameter(g_bus_handle, 0, 1, filter_param1[0]);
  g_soloud.setFilterParameter(g_bus_handle, 0, 2, filter_param2[0]);
}

void RenderSynthEngineSelector() {
  ImGui::SeparatorText("Select Engine");
  static int selection = static_cast<int>(g_synth_engine);
  ImGui::Combo("Synth Engine", &selection,
    "Generated wave\x00"
    "Padsynth\x00"
    "File sample (AKWF_c604_0024.wav)\x00"
    "Superwave\x00"
    "\x00");
  g_synth_engine = static_cast<SynthEngine>(selection);
}

void RenderSelectedSynthEngine() {
  switch (g_synth_engine) {
    case SynthEngine::kGeneratedWave:
      RenderWaveform();
      break;
    case SynthEngine::kPadsynth:
      RenderPadsynth();
      break;
    case SynthEngine::kSuperwave:
      RenderSuperwave();
      break;
  }
}

void SetupSoloud() {
  g_soloud.init(
    SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
  g_soloud.setGlobalVolume(0.75);
  g_soloud.setPostClipScaler(0.75);
  g_bus_handle = g_soloud.play(g_bus);

  SoLoud::generatePadsynth(
    g_padsynth, kPadsynthAharmonics, g_harmonics.data(), g_bw, g_bws);
}

void SetupFilters() {
  g_filter[0] = new SoLoud::BassboostFilter;
  g_filter[1] = new SoLoud::BiquadResonantFilter;
  g_filter[2] = new SoLoud::DCRemovalFilter;
  g_filter[3] = new SoLoud::EchoFilter;
  g_filter[4] = new SoLoud::FFTFilter;
  g_filter[5] = new SoLoud::FlangerFilter;
  g_filter[6] = new SoLoud::FreeverbFilter;
  g_filter[7] = new SoLoud::LofiFilter;
  g_filter[8] = new SoLoud::RobotizeFilter;
  g_filter[9] = new SoLoud::WaveShaperFilter;
  g_filter[10] = new SoLoud::EqFilter;
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

    ImGui::Begin("##Piano Demo", &running, window_flags);

    if (!running) {
      window->Close();
    }

    SetSoloudFilterParams();
    HandleKeys();
    RenderSynthEngineSelector();
    RenderInfoWindow();
    RenderFilterWindow();
    RenderSelectedSynthEngine();

    ImGui::End();
  }
};

}  // namespace

int main() {
  // Soloud Setup
  SetupSoloud();
  SetupFilters();

  // Window Setup
  soloud::tests::common::Window window({"Piano Demo", 640, 1000});
  window.AddRenderable<Content>();
  window.Run();
}