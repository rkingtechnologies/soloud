#ifndef RKT_SOLOUD_DEMOS_COMMON_SIMPLE_WINDOW_H_
#define RKT_SOLOUD_DEMOS_COMMON_SIMPLE_WINDOW_H_

#include "common/window.h"

class SimpleWindow : public soloud::tests::common::Renderable {
 public:
  SimpleWindow(void (*simple_render)()) : simple_render_(simple_render) {}

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

    ImGui::Begin("##simple_window", &running, window_flags);

    if (!running) {
      window->Close();
    }

    simple_render_();

    ImGuiIO& io = ImGui::GetIO();

    ImGui::Separator();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
      1000.0f / io.Framerate, io.Framerate);

    ImGui::End();
  }
  private:
    void (*simple_render_)() = nullptr;
};

#endif  // RKT_SOLOUD_DEMOS_COMMON_SIMPLE_WINDOW_H_

