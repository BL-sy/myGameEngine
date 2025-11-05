#pragma once
#include "Hazel/Input.h"

namespace Hazel {
    class WindowsInput : public Input {
    protected:
        // 重写基类纯虚函数，实现Windows平台的输入查询
        virtual bool IsKeyPressedImpl(int keycode) override;
        virtual bool IsMouseButtonPressedImpl(int button) override;
        virtual std::pair<float, float> GetMousePositionImpl() override;
        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
    };
}