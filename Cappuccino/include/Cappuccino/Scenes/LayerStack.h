#pragma once

#include "Cappuccino/Scenes/Layer.h"

namespace Capp {

	class LayerStack {

		using LayerVector = std::vector<Layer*>;
		using LayerIterator = LayerVector::iterator;
		using ConstLayerIterator = LayerVector::const_iterator;

	public:

		LayerStack() = default;
		~LayerStack();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popLayer(Layer* layer);
		void popOverlay(Layer* overlay);

		LayerIterator begin();
		ConstLayerIterator begin() const;
		LayerIterator end();
		ConstLayerIterator end() const;

	private:

		LayerVector _layers;
		unsigned int _layerInsert = 0;
	};
	
}