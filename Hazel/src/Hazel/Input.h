#pragma once
#include "Hazel/Core.h"
#include <utility> // 用于std::pair

#include "KeyCodes.h"
#include "MouseButtonCodes.h"

namespace Hazel {
    class HAZEL_API Input {
    public:
        // 全局静态接口：键盘按键查询（返回是否按下或重复触发）
        inline static bool IsKeyPressed(int keycode) {
            return s_Instance->IsKeyPressedImpl(keycode);
        }
        // 全局静态接口：鼠标按钮查询（返回是否按下）
        inline static bool IsMouseButtonPressed(int button) {
            return s_Instance->IsMouseButtonPressedImpl(button);
        }
        // 全局静态接口：获取鼠标位置（返回X、Y坐标对）
        inline static std::pair<float, float> GetMousePosition() {
            return s_Instance->GetMousePositionImpl();
        }
        // 全局静态接口：单独获取鼠标X坐标
        inline static float GetMouseX() {
            return s_Instance->GetMouseXImpl();
        }
        // 全局静态接口：单独获取鼠标Y坐标
        inline static float GetMouseY() {
            return s_Instance->GetMouseYImpl();
        }

    protected:
        // 纯虚函数：由平台实现类（如WindowsInput）重写
        virtual bool IsKeyPressedImpl(int keycode) = 0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual std::pair<float, float> GetMousePositionImpl() = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;

    private:
        // 静态单例指针：指向具体平台的输入实例（如WindowsInput）
        static Input* s_Instance;
    };
}