#pragma once

#include <chrono>
#include <random>

namespace Capp {

	class Random {

		using HRC_TimePoint = std::chrono::high_resolution_clock::time_point;
		using HRC_Duration = std::chrono::high_resolution_clock::time_point;
		using DefaultRandomEngine = std::default_random_engine;
		
	public:

		static int Int(int low = 0, int high = 10);
		static float Float(float low = 0.0f, float high = 1.0f);
		static double Double(double low = 0.0, double high = 1.0);

	private:

		static HRC_TimePoint _timeNow;
		static HRC_Duration _seed;
		static DefaultRandomEngine _generator;
		
	};
	
}