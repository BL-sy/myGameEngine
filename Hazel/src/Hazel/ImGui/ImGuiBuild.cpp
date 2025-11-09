#include "hzpch.h"
// 指定使用GLAD作为OpenGL加载器，需确保Hazel已集成GLAD
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
// 引入ImGui官方GLFW后端（处理窗口事件）和OpenGL3后端（处理渲染）
#include "backends/imgui_impl_glfw.cpp"
#include "backends/imgui_impl_opengl3.cpp"