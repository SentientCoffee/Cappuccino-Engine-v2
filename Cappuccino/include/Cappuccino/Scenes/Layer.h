#pragma once

#include "Cappuccino/Events/Event.h"

namespace Capp {

	class LayerStack;

	class Layer {

		friend LayerStack;
		
	public:

		Layer(const std::string& name);
		virtual ~Layer() = default;

		virtual void onPush();
		virtual void onPop();
		virtual void update();
		virtual void drawImgui();
		virtual void onEvent(Event& e);

		const std::string& getName() const;

	protected:

		std::string _layerName;
		
	};
	
}