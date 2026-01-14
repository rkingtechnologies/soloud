#ifndef SOLOUD_TESTS_COMMON_THEMES_H_
#define SOLOUD_TESTS_COMMON_THEMES_H_

#include "imgui.h"

/*!
 * @brief Namespace containing all classes, structs, functions, etc. used for
 * managing and creating custom ImGui themes.
 */
namespace soloud::tests::common::themes {

/*!
 * @brief Sets ImGui style and color parameters for a modern dark theme.
 * @details Used in place of ImGui::StyleColorsDark();
 */
inline void SetModernDarkTheme() {
  ImGuiStyle& style = ImGui::GetStyle();
  ImVec4* colors = style.Colors;

  colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.55f, 0.60f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.13f, 0.15f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.10f, 0.98f);
  colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.28f, 0.50f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.24f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.27f, 0.30f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.32f, 0.36f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.17f, 0.19f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.09f, 0.75f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.15f, 0.17f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.35f, 0.38f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.43f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.45f, 0.45f, 0.48f, 1.00f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.93f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.33f, 0.67f, 0.93f, 1.00f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.47f, 0.77f, 1.00f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.25f, 0.27f, 0.30f, 1.00f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.37f, 0.40f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.42f, 0.46f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.25f, 0.28f, 0.32f, 1.00f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.33f, 0.36f, 0.41f, 1.00f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.42f, 0.47f, 1.00f);
  colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.28f, 0.50f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.33f, 0.67f, 0.93f, 1.00f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.47f, 0.77f, 1.00f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.33f, 0.67f, 0.93f, 0.25f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.33f, 0.67f, 0.93f, 0.67f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.47f, 0.77f, 1.00f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.17f, 0.20f, 1.00f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.67f, 0.93f, 0.80f);
  colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.23f, 0.27f, 1.00f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.17f, 0.20f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.23f, 0.27f, 1.00f);
  colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.93f, 0.78f);
  colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.07f, 0.07f, 0.08f, 1.00f);
  colors[ImGuiCol_PlotLines] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.33f, 0.67f, 0.93f, 1.00f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.33f, 0.67f, 0.93f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.47f, 0.77f, 1.00f, 1.00f);
  colors[ImGuiCol_TableHeaderBg] = ImVec4(0.18f, 0.20f, 0.22f, 1.00f);
  colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
  colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.26f, 1.00f);
  colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.12f, 0.12f, 0.13f, 0.50f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.33f, 0.67f, 0.93f, 0.35f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.93f, 1.00f);
  colors[ImGuiCol_NavHighlight] = ImVec4(0.33f, 0.67f, 0.93f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.75f);

  style.Alpha = 1.0f;
  style.DisabledAlpha = 0.6f;
  style.WindowPadding = ImVec2(8.0f, 8.0f);
  style.WindowRounding = 6.0f;
  style.WindowBorderSize = 1.0f;
  style.WindowMinSize = ImVec2(32.0f, 32.0f);
  style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
  style.WindowMenuButtonPosition = ImGuiDir_Left;
  style.ChildRounding = 4.0f;
  style.ChildBorderSize = 1.0f;
  style.PopupRounding = 4.0f;
  style.PopupBorderSize = 1.0f;
  style.FramePadding = ImVec2(6.0f, 4.0f);
  style.FrameRounding = 5.0f;
  style.FrameBorderSize = 0.0f;
  style.ItemSpacing = ImVec2(8.0f, 6.0f);
  style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
  style.CellPadding = ImVec2(6.0f, 4.0f);
  style.IndentSpacing = 21.0f;
  style.ColumnsMinSpacing = 6.0f;
  style.ScrollbarSize = 14.0f;
  style.ScrollbarRounding = 9.0f;
  style.GrabMinSize = 12.0f;
  style.GrabRounding = 5.0f;
  style.TabRounding = 5.0f;
  style.TabBorderSize = 0.0f;
  style.ColorButtonPosition = ImGuiDir_Right;
  style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
  style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
  style.DisplayWindowPadding = ImVec2(20.0f, 20.0f);
  style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);
  style.MouseCursorScale = 1.0f;
  style.AntiAliasedLines = true;
  style.AntiAliasedLinesUseTex = true;
  style.AntiAliasedFill = true;
  style.CurveTessellationTol = 1.25f;
  style.CircleTessellationMaxError = 0.30f;
}

}  // namespace soloud::tests::common::themes

#endif  // SOLOUD_TESTS_COMMON_THEMES_H_