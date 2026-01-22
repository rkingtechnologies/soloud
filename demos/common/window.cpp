#include "window.h"

#include <filesystem>
#include <format>
#include <iterator>
#include <print>
#include <string>

// #include "error_handler.h"
// #include "fonts.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "themes.h"

namespace soloud::tests::common {

namespace {

// Compile-time string constants.
constexpr std::string_view kGlslVersion = "#version 130";
constexpr int kGlfwMajorVersion = 3;
constexpr int kGlfwMinorVersion = 3;
constexpr int kVsyncInterval = 1;
constexpr int kMinimizedWindowSleepMS = 10;
constexpr ImVec4 kClearColor{0.0f, 0.0f, 0.0f, 0.8f};

void GlfwErrorCallback(int error, const char* description) {
  auto err_msg = std::format("GLFW Error {}: {}", error, description);
  std::println("{}", std::move(err_msg));
}

bool InitializeGlfwContext(float& out_main_scale) {
  if (!glfwInit()) {
    std::println("Failed to initialize GLFW");
    return false;
  }

  GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
  if (!primary_monitor) {
    glfwTerminate();
    std::println("Failed to get primary monitor");
    return false;
  }

  out_main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(primary_monitor);
  return true;
}
}  // namespace

Window::Window(const WindowOptions& options)
    : options_(options), is_initialized_(false), is_running_(false) {
  glfwSetErrorCallback(GlfwErrorCallback);

  float main_scale = 1.0f;
  if (!InitializeGlfwContext(main_scale)) {
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kGlfwMajorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kGlfwMinorVersion);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  const int scaled_width = static_cast<int>(main_scale * options_.width);
  const int scaled_height = static_cast<int>(main_scale * options_.height);

  // Let the smart pointer manage the raw pointer from creation.
  window_.reset(glfwCreateWindow(
    scaled_width, scaled_height, options_.title, nullptr, nullptr));

  if (!window_) {
    glfwTerminate();
    std::println("Failed to create GLFW window");
    return;
  }

  glfwMakeContextCurrent(window_.get());
  glfwSwapInterval(kVsyncInterval);

  InitializeImGui(main_scale);
  InitializeImGuiBackends();

  is_initialized_ = true;
}

void Window::InitializeImGui(float main_scale) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  themes::SetModernDarkTheme();

  ImGuiStyle& style = ImGui::GetStyle();
  style.ScaleAllSizes(main_scale);

  io.ConfigDpiScaleViewports = true;

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
}

void Window::InitializeImGuiBackends() {
  ImGui_ImplGlfw_InitForOpenGL(window_.get(), true);
  ImGui_ImplOpenGL3_Init(kGlslVersion.data());

  ImGuiIO& io = ImGui::GetIO();

  default_font_ = io.Fonts->AddFontDefault();
  try {
    regular_font_ =
      io.Fonts->AddFontFromFileTTF("../assets/fonts/lato_regular.ttf", 24);
    bold_font_ =
      io.Fonts->AddFontFromFileTTF("../assets/fonts/lato_bold.ttf", 24);
  } catch (...) {
    regular_font_ = nullptr;
    bold_font_ = nullptr;
  }
}

void Window::Run() {
  if (!is_initialized_) {
    std::println("Run() called but window is not properly initialized");
    return;
  }

  is_running_ = true;

  while (is_running_ && !glfwWindowShouldClose(window_.get())) {
    glfwPollEvents();

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window_.get(), &width, &height);

    const bool is_iconified =
      glfwGetWindowAttrib(window_.get(), GLFW_ICONIFIED) != 0;

    if (width <= 0 || height <= 0 || is_iconified) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::PushFont(RegularFont());

    RenderUI();

    ImGui::PopFont();

    ImGui::Render();

    glViewport(0, 0, width, height);
    glClearColor(kClearColor.x, kClearColor.y, kClearColor.z, kClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    HandleMultipleViewports();

    glfwSwapBuffers(window_.get());
  }

  is_running_ = false;
}

void Window::RenderUI() {
  std::erase_if(renderables_, [this](const auto& renderable) {
    if (!renderable->IsRendering()) {
      return true;  // Mark for removal.
    }
    renderable->Render(this);
    return false;  // Keep in list.
  });
}

void Window::RenderFrame() {
  int display_w, display_h;
  glfwGetFramebufferSize(window_.get(), &display_w, &display_h);

  glViewport(0, 0, display_w, display_h);
  glClearColor(kClearColor.x, kClearColor.y, kClearColor.z, kClearColor.w);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::HandleMultipleViewports() {
  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_context);
  }
}

void Window::Close() {
  is_running_ = false;
}

void Window::Cleanup() {
  if (!window_) {
    return;
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  if (ImGui::GetCurrentContext()) {
    ImGui::DestroyContext();
  }

  window_.reset();

  if (is_initialized_) {
    glfwTerminate();
  }
}

Window::~Window() {
  Cleanup();
}

}  // namespace soloud::tests::common