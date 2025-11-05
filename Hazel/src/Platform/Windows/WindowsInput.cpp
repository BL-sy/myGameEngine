#include "hzpch.h"
#include "WindowsInput.h"
#include "Hazel/Application.h"
#include <GLFW/glfw3.h> // 引入GLFW头文件

namespace Hazel {
    // 初始化单例：让基类指针指向Windows平台的输入实例
    Input* Input::s_Instance = new WindowsInput();

    // 键盘按键查询实现：调用glfwGetKey获取状态
    bool WindowsInput::IsKeyPressedImpl(int keycode) {
        // 将引擎封装的窗口句柄转为GLFW原生句柄
        auto window = static_cast<GLFWwindow*>(
            Application::Get().GetWindow().GetNativeWindow()
            );
        // GLFW_KEY_PRESS：单次按下；GLFW_REPEAT：长按重复触发
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    // 鼠标按钮查询实现：调用glfwGetMouseButton获取状态
    bool WindowsInput::IsMouseButtonPressedImpl(int button) {
        auto window = static_cast<GLFWwindow*>(
            Application::Get().GetWindow().GetNativeWindow()
            );
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    // 鼠标位置查询实现：调用glfwGetCursorPos获取坐标
    std::pair<float, float> WindowsInput::GetMousePositionImpl() {
        auto window = static_cast<GLFWwindow*>(
            Application::Get().GetWindow().GetNativeWindow()
            );
        double xpos, ypos; // GLFW返回double类型，需转为float
        glfwGetCursorPos(window, &xpos, &ypos);
        return { static_cast<float>(xpos), static_cast<float>(ypos) };
    }

    // 单独获取鼠标X坐标：复用GetMousePositionImpl的结果
    float WindowsInput::GetMouseXImpl() {
        // C++17结构化绑定：直接从坐标对中提取X值
        auto [x, y] = GetMousePositionImpl();
        return x;
        // 兼容C++14及以下：使用std::get<0>提取第一个元素
        // auto pos = GetMousePositionImpl();
        // return std::get<0>(pos);
    }

    // 单独获取鼠标Y坐标：同理复用已有逻辑
    float WindowsInput::GetMouseYImpl() {
        auto [x, y] = GetMousePositionImpl();
        return y;
    }
}