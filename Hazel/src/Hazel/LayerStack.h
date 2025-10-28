#pragma once

#include "Hazel/Layer.h"

namespace Hazel {
    class HAZEL_API LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        // ��Ӳ㣨�����ȼ����֣���ͨ���β���ӣ�overlay���ͷ���ӣ�
        void PushLayer(Layer* layer); // ��ͨ�㣨����Ϸ�㡢UI�㣩
        void PushOverlay(Layer* overlay); // ���ǲ㣨����Բ㡢�����㣬���ȼ����ߣ�
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        // ������֧�֣����ڱ������ջ
        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }
    private:
        // �洢���в��������������ͨ�� + Overlay��
        std::vector<Layer*> m_Layers;
        // �����ķֽ��������ָ����ͨ���ĩβ��Overlay �Ŀ�ͷ
        // ���ã�ȷ����ͨ�����ǰ�棬Overlay ���ں��棬������
        std::vector<Layer*>::iterator m_LayerInsert;
    };
}

