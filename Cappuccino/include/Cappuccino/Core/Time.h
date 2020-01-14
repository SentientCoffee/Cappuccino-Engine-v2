#pragma once

namespace Capp {

	class Time {
	public:
		
		static float getTime();
		static float getDeltaTime();
		static void calculateDeltaTime();

	private:

		static float _currentFrameTime;
		static float _lastFrameTime;
		
	};
	
}