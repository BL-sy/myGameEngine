#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {
    // ���캯������ʼ���ֽ��������ָ���ջ��ͷ����ʱ���κβ㣬��ͨ������Ϊ�գ�
    LayerStack::LayerStack()
    {
        m_LayerInsert = m_Layers.begin();
    }

    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_Layers)
            delete layer;
    }

    // ������ͨ�㣺
    // 1. emplace �� m_LayerInsert λ�ã���ͨ�������ĩβ��
    // 2. �������� m_LayerInsert������ָ���µ���ͨ��ĩβ���´β���ͨ��������ȷλ�ã�
    void LayerStack::PushLayer(Layer* layer)
    {
        // emplace �����²���Ԫ�صĵ���������ֵ�� m_LayerInsert ��ɸ���
        m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
    }

    // ���� Overlay��ֱ�Ӳ��ڲ�ջĩβ��Overlay ���򣩣���Ӱ�� m_LayerInsert
    void LayerStack::PushOverlay(Layer* overlay)
    {
        m_Layers.emplace_back(overlay);
    }

    // �Ƴ���ͨ�㣺���� m_LayerInsert����ֹ������ʧЧ��
    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
            m_LayerInsert = it;  // �ؼ���ɾ������·ֽ�����������������ָ����Чλ��
        }
    }

    // �Ƴ� Overlay��Overlay ��ĩβ����Ӱ�� m_LayerInsert
    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
        }
    }
}