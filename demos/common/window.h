#ifndef SOLOUD_TESTS_COMMON_WINDOW_H_
#define SOLOUD_TESTS_COMMON_WINDOW_H_

#pragma warning(disable: 4005)
#include <GLFW/glfw3.h>

#include "imgui.h"

#include <cstdint>
#include <list>
#include <memory>
#include <string>

namespace soloud::tests::common {

/*!
 * @brief Struct containing options for creating a Window instance.
 * @param title The window title.
 * @param width The window width in pixels.
 * @param height The window height in pixels.
 */
struct WindowOptions {
  const char* title;
  uint_fast16_t width;
  uint_fast16_t height;
};

class Renderable;
class Window;

/*!
 * @brief Base class for objects that can be rendered by the Window.
 *
 * @details Derived classes should override Render() to implement custom
 * rendering logic. The Window will call Render() every frame and automatically
 * remove Renderables where is_rendering_ is false.
 */
class Renderable {
 public:
  virtual ~Renderable() = default;

  bool IsRendering() const noexcept {
    return is_rendering_;
  }
  void StopRendering() noexcept {
    is_rendering_ = false;
  }

 protected:
  /*!
   * @brief Pure virtual method called each frame for rendering.
   * @param window Pointer to the Window that is rendering this object.
   */
  virtual void Render(Window* window) = 0;

  friend class Window;

  bool is_rendering_ = true;
};

/*!
 * @brief Class responsible for managing the GLFW window and ImGui context.
 *
 * @details Contains the main render loop that calls all Renderable objects
 * in the render list. This class is non-copyable and non-movable to prevent
 * multiple management of the same window resource.
 */
class Window {
 private:
  struct GlfwWindowDeleter {
    void operator()(GLFWwindow* ptr) const {
      if (ptr) {
        glfwDestroyWindow(ptr);
      }
    }
  };

 public:
  explicit Window(const WindowOptions& options);
  ~Window();

  // Deleted copy and move semantics for strict ownership.
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  Window(Window&&) noexcept = delete;
  Window& operator=(Window&&) noexcept = delete;

  bool IsInitialized() const noexcept {
    return is_initialized_;
  }
  bool IsRunning() const noexcept {
    return is_running_;
  }
  GLFWwindow* GetHandle() const noexcept {
    return window_.get();
  }

  inline ImFont* RegularFont() {
    if (regular_font_ != nullptr) {
      return regular_font_;
    }
    return default_font_;
  }

  inline ImFont* BoldFont() {
    if (bold_font_ != nullptr) {
      return bold_font_;
    }
    return default_font_;
  }

  /*!
   * @brief Adds a new Renderable to the render list.
   * @tparam T Must derive from Renderable.
   * @tparam Args Parameter types for T's constructor.
   * @param args Arguments forwarded to T's constructor.
   */
  template <typename T, typename... Args>
  void AddRenderable(Args&&... args) {
    static_assert(
      std::is_base_of_v<Renderable, T>, "T must derive from Renderable");
    renderables_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
  }

  void Run();
  void Close();

  size_t GetRenderableCount() const noexcept {
    return renderables_.size();
  }

 private:
  void Cleanup();
  void InitializeImGui(float main_scale);
  void InitializeImGuiBackends();
  void RenderUI();
  void RenderFrame();
  void HandleMultipleViewports();

  WindowOptions options_;
  std::unique_ptr<GLFWwindow, GlfwWindowDeleter> window_{nullptr};
  bool is_initialized_ = false;
  bool is_running_ = false;

  std::list<std::unique_ptr<Renderable>> renderables_;

  ImFont* default_font_;
  ImFont* regular_font_;
  ImFont* bold_font_;
};

}  // namespace soloud::tests::common

#endif  // SOLOUD_TESTS_COMMON_WINDOW_H_