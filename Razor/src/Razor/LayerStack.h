#pragma once

#include "Razor/Core.h"
#include "Layer.h"

namespace Razor {

	class RAZOR_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		typedef std::vector<Layer*> LayerList;
		typedef std::vector<Layer*>::iterator LayerIter;

		LayerIter begin() { return m_Layers.begin(); }
		LayerIter end() { return m_Layers.end(); }

	private:
		LayerList m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};

}