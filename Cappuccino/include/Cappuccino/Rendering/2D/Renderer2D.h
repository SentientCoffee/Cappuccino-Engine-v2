#pragma once

#include "Cappuccino/Rendering/Camera.h"
#include "Cappuccino/Rendering/2D/2DQuad.h"
#include "Cappuccino/Rendering/Text/Text.h"

namespace Capp {

	class Renderer2D {
	public:

		static void init();
		static void shutdown();

		static void onWindowResized(uint32_t width, uint32_t height);
		
		static void start();
		static void start(const OrthographicCamera& camera);
		static void finish();

		static void drawQuad(const Quad& quad);

		static void drawText(const Ref<Text>& text);
		
	};
	
}
