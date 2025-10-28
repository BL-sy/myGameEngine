#pragma once

namespace Hazel {
	class Layer;
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
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};
}

