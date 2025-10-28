#pragma once

#include "Hazel/Layer.h"

namespace Hazel {
    class HAZEL_API LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        // 添加层（按优先级区分：普通层从尾部加，overlay层从头部加）
        void PushLayer(Layer* layer); // 普通层（如游戏层、UI层）
        void PushOverlay(Layer* overlay); // 覆盖层（如调试层、弹窗层，优先级更高）
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        // 迭代器支持，用于遍历层堆栈
        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }
    private:
        // 存储所有层的容器（包含普通层 + Overlay）
        std::vector<Layer*> m_Layers;
        // 【核心分界迭代器】指向普通层的末尾、Overlay 的开头
        // 作用：确保普通层插在前面，Overlay 插在后面，不混淆
        std::vector<Layer*>::iterator m_LayerInsert;
    };
}

