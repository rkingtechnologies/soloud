#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "common/simple_window.h"
#include "common/window.h"
#include "imgui.h"
#include "soloud.h"
#include "soloud_sfxr.h"
#include "soloud_speech.h"

namespace {

SoLoud::Soloud gSoloud;
SoLoud::Sfxr gSfx_mouse, gSfx_orbit;
SoLoud::Speech gSfx_crazy;

int gSndHandle_mouse = 0;
int gSndHandle_orbit = 0;
int gSndHandle_crazy = 0;

bool orbit_enable = 1;
bool crazy_enable = 1;
bool mouse_enable = 1;

void DemoTriangle(
  float x0, float y0, float x1, float y1, float x2, float y2, ImU32 color) {
  ImGui::GetWindowDrawList()->AddTriangleFilled(
    ImVec2(x0, y0), ImVec2(x1, y1), ImVec2(x2, y2), color);
}

void Render2D() {
  // 1. Layout Calculations
  // ────────────────────────────────────────────────
  float sidebar_width = 260.0f;
  ImVec2 win_pos = ImGui::GetWindowPos();
  ImVec2 win_size = ImGui::GetWindowSize();

  // Calculate the center of the DRAWING area (Canvas)
  float cx = win_pos.x + sidebar_width + (win_size.x - sidebar_width) * 0.5f;
  float cy = win_pos.y + win_size.y * 0.5f;

  // 2. Audio Logic (Original Orientation Restored)
  // ────────────────────────────────────────────────
  gSoloud.setPause(gSndHandle_crazy, !crazy_enable);
  gSoloud.setPause(gSndHandle_orbit, !orbit_enable);
  gSoloud.setPause(gSndHandle_mouse, !mouse_enable);

  float tick = (float)ImGui::GetTime();
  float tickd = tick - 0.1f;

  // --- Crazy Sound ---
  float crazyx =
    (float)(sin(tick) * sin(tick * 0.234) * sin(tick * 4.234) * 150);
  float crazyz =
    (float)(cos(tick) * cos(tick * 0.234) * cos(tick * 4.234) * 150 - 50);
  float crazyxv =
    (float)(sin(tickd) * sin(tickd * 0.234) * sin(tickd * 4.234) * 150) -
    crazyx;
  float crazyzv =
    (float)(cos(tickd) * cos(tickd * 0.234) * cos(tickd * 4.234) * 150 - 50) -
    crazyz;

  // Back to original: (x, 0, z) logic
  // Parameters: (handle, x, y, z, vx, vy, vz)
  gSoloud.set3dSourceParameters(
    gSndHandle_crazy, crazyx, 0, crazyz, crazyxv, 0, crazyzv);

  // --- Orbit Sound ---
  float orbitx = (float)sin(tick) * 50;
  float orbitz = (float)cos(tick) * 50;
  float orbitxv = (float)sin(tickd) * 50 - orbitx;
  float orbitzv = (float)cos(tickd) * 50 - orbitz;

  gSoloud.set3dSourceParameters(
    gSndHandle_orbit, orbitx, 0, orbitz, orbitxv, 0, orbitzv);

  // --- Mouse Sound ---
  ImVec2 m = ImGui::GetMousePos();
  // Sound relative to the center triangle:
  // Screen Y becomes Audio Z to maintain the "Depth" feel of your original code
  float mouse_audio_x = m.x - cx;
  float mouse_audio_z = m.y - cy;
  gSoloud.set3dSourcePosition(
    gSndHandle_mouse, mouse_audio_x, 0, mouse_audio_z);

  gSoloud.update3dAudio();

  // 3. UI Rendering (The Sidebar)
  // ────────────────────────────────────────────────
  ImGui::BeginChild("ControlRegion", ImVec2(sidebar_width, 0), true);
  ImGui::SeparatorText("Output");
  float* buf = gSoloud.getWave();
  float* fft = gSoloud.calcFFT();
  ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(-1, 80));
  ImGui::PlotHistogram("##FFT", fft, 128, 0, "FFT", 0, 10, ImVec2(-1, 80));

  ImGui::SeparatorText("Controls");
  ImGui::Checkbox("Orbit sound", &orbit_enable);
  ImGui::Checkbox("Crazy sound", &crazy_enable);
  ImGui::Checkbox("Mouse sound", &mouse_enable);
  ImGui::EndChild();

  ImGui::SameLine();

  // 4. Drawing Rendering (The Canvas)
  // ────────────────────────────────────────────────
  ImGui::BeginChild("CanvasRegion", ImVec2(0, 0), false);
  ImDrawList* dl = ImGui::GetWindowDrawList();

  // Center Triangle
  DemoTriangle(5 + cx, 5 + cy - 20, 5 + cx - 20, 5 + cy + 20, 5 + cx + 20,
    5 + cy + 20, 0x77000000);
  DemoTriangle(cx, cy - 20, cx - 20, cy + 20, cx + 20, cy + 20, 0xffeeeeee);

  // Orbit (Screen X = orbitx, Screen Y = orbitz)
  DemoTriangle(5 + cx + orbitx * 2, 5 + cy + orbitz * 2 - 10,
    5 + cx + orbitx * 2 - 10, 5 + cy + orbitz * 2 + 10,
    5 + cx + orbitx * 2 + 10, 5 + cy + orbitz * 2 + 10, 0x77000000);
  DemoTriangle(cx + orbitx * 2, cy + orbitz * 2 - 10, cx + orbitx * 2 - 10,
    cy + orbitz * 2 + 10, cx + orbitx * 2 + 10, cy + orbitz * 2 + 10,
    0xffffff00);

  // Crazy (Screen X = crazyx, Screen Y = crazyz)
  DemoTriangle(5 + cx + crazyx * 2, 5 + cy + crazyz * 2 - 10,
    5 + cx + crazyx * 2 - 10, 5 + cy + crazyz * 2 + 10,
    5 + cx + crazyx * 2 + 10, 5 + cy + crazyz * 2 + 10, 0x77000000);
  DemoTriangle(cx + crazyx * 2, cy + crazyz * 2 - 10, cx + crazyx * 2 - 10,
    cy + crazyz * 2 + 10, cx + crazyx * 2 + 10, cy + crazyz * 2 + 10,
    0xffff00ff);

  // Mouse
  DemoTriangle(5 + m.x, 5 + m.y - 10, 5 + m.x - 10, 5 + m.y + 10, 5 + m.x + 10,
    5 + m.y + 10, 0x77000000);
  DemoTriangle(
    m.x, m.y - 10, m.x - 10, m.y + 10, m.x + 10, m.y + 10, 0xff00ffff);
  ImGui::EndChild();
}

void InitAudio() {
  gSoloud.init(
    SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
  gSoloud.setGlobalVolume(4);

  gSfx_mouse.loadPreset(SoLoud::Sfxr::LASER, 3);
  gSfx_mouse.setLooping(1);
  gSfx_mouse.set3dMinMaxDistance(1, 200);
  gSfx_mouse.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.5);
  gSndHandle_mouse = gSoloud.play3d(gSfx_mouse, 100, 0, 0);

  gSfx_orbit.loadPreset(SoLoud::Sfxr::COIN, 3);
  gSfx_orbit.setLooping(1);
  gSfx_orbit.set3dMinMaxDistance(1, 200);
  gSfx_orbit.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.5);
  gSndHandle_orbit = gSoloud.play3d(gSfx_orbit, 50, 0, 0);

  gSfx_crazy.setText(
    "I'm going into space with my space ship space ship space ship "
    "spaceeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
  gSfx_crazy.setLooping(1);
  gSfx_crazy.set3dMinMaxDistance(1, 400);
  gSfx_crazy.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.25);
  gSndHandle_crazy = gSoloud.play3d(gSfx_crazy, 50, 0, 0);
}

}  // namespace

int main() {
  InitAudio();
  SimpleWindow simple_window(Render2D);
  soloud::tests::common::Window window({"2D Demo", 800, 600});
  window.AddRenderable<SimpleWindow>(simple_window);
  window.Run();
}