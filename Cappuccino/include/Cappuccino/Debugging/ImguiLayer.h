#pragma once

#include "Cappuccino/Scenes/Layer.h"

namespace Capp {

	class ImguiLayer : public Layer {
	public:

		ImguiLayer();
		~ImguiLayer() = default;

		void onPush() override;
		void onPop() override;
		void drawImgui() override;

		static void begin();
		static void end();
		
	};
	
}