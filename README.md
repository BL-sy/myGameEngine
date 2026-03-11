# 自研 2D 游戏引擎

一个基于 C++17 和 OpenGL 的轻量级 2D 游戏引擎，支持批处理渲染、事件系统、ImGui 调试界面等核心功能。
本项目是我个人学习游戏引擎架构的实践项目，参考了 The Cherno 的 Hazel 引擎系列教程，并在此基础上进行了扩展和优化。


✨ 特性

核心架构

· 跨平台窗口：基于 GLFW 的窗口抽象，支持 Windows / macOS / Linux。
· 事件系统：基于委托的事件分发机制，支持窗口事件、键盘/鼠标事件、应用事件。
· 日志系统：集成 spdlog，支持多级别日志输出（引擎日志 / 客户端日志）。
· 层（Layer）系统：支持叠加式 UI 层，便于功能模块化。
· 预编译头：优化编译速度。

渲染系统

· OpenGL 渲染 API 抽象：封装顶点缓冲区、索引缓冲区、顶点数组、着色器、纹理。
· 2D 渲染器：
  · 支持精灵（Quad）绘制，可设置颜色、纹理、变换。
  · 批处理渲染：将多个精灵合并为单个 Draw Call，大幅提升性能。
  · 纹理图集（Texture Atlas）支持。
· 摄像机：正交相机控制器，支持平移/旋转/缩放。
· 着色器：支持 GLSL 着色器文件加载，统一变量（Uniform）管理。

调试与工具

· ImGui 集成：内置 Dear ImGui，提供运行时调试界面。
  · 视口停靠、性能监控、场景参数调整。
  · 支持 Docking 和 Viewport。

资源管理

· 纹理库、着色器库：单例模式管理资源，避免重复加载。
· 智能指针管理资源生命周期（RAII）。

---

🛠️ 技术栈

领域 技术
编程语言 C++17
图形 API OpenGL 4.5 + GLAD
窗口管理 GLFW
数学库 glm
日志 spdlog
UI 调试 Dear ImGui
构建系统 Premake5

---

🚀 快速开始

环境要求

· 支持 C++17 的编译器（Visual Studio 2019+ / GCC 9+ / Clang 10+）
· Premake5（下载地址）
· Git

克隆仓库

```bash
git clone https://github.com/你的用户名/引擎仓库名.git
cd 引擎仓库名
```

生成项目文件（以 Windows + Visual Studio 为例）

```bash
cd scripts
./Win_GenProjects.bat        # 会自动调用 Premake 生成 VS 解决方案
```


编译运行

· 用 Visual Studio 打开生成的 .sln 文件。
· 编译并运行，即可看到引擎窗口和 ImGui 界面。

---

📁 项目结构

```
引擎根目录/
├── Hazel/                 	# 引擎核心代码
│   ├── src/                # 头文件和实现
│   ├── vendor/             # 第三方库 (spdlog, glm, imgui 等)
├── Sandbox/                # 示例项目/沙盒
│   ├── src/                # 测试代码
│   └── assets/       		# 着色器、纹理等
├── vendor/					# premake5.exe
├── scripts/                # 辅助脚本
├── premake5.lua            # 根构建脚本
├── README.md
└── LICENSE
```

---

🔧 主要功能演示（代码片段）

创建一个简单的应用

```cpp
#include "Engine/Engine.h"

class ExampleLayer : public Engine::Layer
{
public:
    void OnUpdate(Engine::Timestep ts) override
    {
        // 更新逻辑
    }
    
    void OnEvent(Engine::Event& event) override
    {
        // 事件处理
    }
};

class Sandbox : public Engine::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }
};

Engine::Application* Engine::CreateApplication()
{
    return new Sandbox();
}
```

使用 2D 渲染器绘制精灵

```cpp
// 在渲染循环中
Hazel::Renderer2D::BeginScene(camera);
Hazel::Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.8f, 0.2f, 1.0f}); // 颜色
Hazel::Renderer2D::DrawQuad({1.2f, 0.0f}, {1.0f, 1.0f}, texture); // 纹理
Hazel::Renderer2D::EndScene();
```

---

📈 未来计划

· 添加物理引擎集成（Box2D）
· 支持音频系统（OpenAL 或 FMOD）
· 粒子系统
· 场景序列化（JSON/YAML）
· 3D 渲染器基础

---

📄 许可证

本项目采用 MIT 许可证。详情请查看 LICENSE 文件。

---

🙏 致谢

· The Cherno – Hazel 引擎系列教程，为本项目提供了宝贵的学习资源。
· GLFW、GLAD、Dear ImGui、spdlog 等开源库的开发者。

---

📬 联系

· 邮箱：[3026709446@qq.com]
· B站：[https://space.bilibili.com/430912952]

如果觉得这个项目对你有帮助，欢迎给个 ⭐️！

---