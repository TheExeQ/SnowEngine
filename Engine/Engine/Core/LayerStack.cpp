#include "LayerStack.h"

namespace Snow
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		myLayers.emplace(myLayers.begin() + myLayerInsertIndex, layer);
		myLayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		myLayers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(myLayers.begin(), myLayers.end(), layer);
		if (it != myLayers.end())
		{
			myLayers.erase(it);
			myLayerInsertIndex--;
		}

	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(myLayers.begin(), myLayers.end(), overlay);
		if (it != myLayers.end())
			myLayers.erase(it);
	}
}
