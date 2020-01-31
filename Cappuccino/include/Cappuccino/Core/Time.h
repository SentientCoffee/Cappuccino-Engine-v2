#pragma once

namespace Capp {

	class Time {
	public:
		
		static float getTime();
		static float getDeltaTime();

		static void preUpdate();
		static void postUpdate();

	private:

		static float _deltaTime;
		static float _lastFrameTime;
		static float _totalAppTime;
		
	};
	
}