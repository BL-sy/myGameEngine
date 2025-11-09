#pragma once

namespace Hazel {

	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void Swap() = 0;
	};
}