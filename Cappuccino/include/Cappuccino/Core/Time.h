#pragma once

namespace Capp {

	class Time {

		friend class Application;
		
	public:
		
		static float getTime();
		static float getDeltaTime();

	private:

		static void preUpdate();
		static void postUpdate();

		static float _deltaTime;
		static float _lastFrameTime;
		static float _totalAppTime;
		
	};
	
}