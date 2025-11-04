#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {
    // 构造函数：初始化分界迭代器，指向层栈开头（此时无任何层，普通层区域为空）
    LayerStack::LayerStack()
    {
        m_LayerInsert = m_Layers.begin();
    }

    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_Layers)
            delete layer;
    }

    // 插入普通层：
    // 1. emplace 在 m_LayerInsert 位置（普通层区域的末尾）
    // 2. 插入后更新 m_LayerInsert，保持指向新的普通层末尾（下次插普通层仍在正确位置）
    void LayerStack::PushLayer(Layer* layer)
    {
        // emplace 返回新插入元素的迭代器，赋值给 m_LayerInsert 完成更新
        m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
        layer->OnAttach();
    }

    // 插入 Overlay：直接插在层栈末尾（Overlay 区域），不影响 m_LayerInsert
    void LayerStack::PushOverlay(Layer* overlay)
    {
        m_Layers.emplace_back(overlay);
        overlay->OnAttach();
    }

    // 移除普通层：更新 m_LayerInsert（防止迭代器失效）
    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
            m_LayerInsert = it;  // 关键：删除后更新分界迭代器，避免迭代器指向无效位置
        }
    }

    // 移除 Overlay：Overlay 在末尾，不影响 m_LayerInsert
    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
        }
    }
}