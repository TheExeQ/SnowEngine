#pragma once

#include "Layer.h"

#include <vector>
#include <assert.h>

namespace Snow
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		Layer* operator[](size_t index)
		{
			assert(index >= 0 && index < myLayers.size());
			return myLayers[index];
		}

		const Layer* operator[](size_t index) const
		{
			assert(index >= 0 && index < myLayers.size());
			return myLayers[index];
		}

		size_t Size() const { return myLayers.size(); }

		std::vector<Layer*>::iterator begin() { return myLayers.begin(); }
		std::vector<Layer*>::iterator end() { return myLayers.end(); }
	private:
		std::vector<Layer*> myLayers;
		unsigned int myLayerInsertIndex = 0;
	};
}