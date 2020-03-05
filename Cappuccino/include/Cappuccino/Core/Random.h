#pragma once

#include <random>

namespace Capp {

	class Random {
		
		using MTEngine = std::mt19937;
		using IntDistribution = std::uniform_int_distribution<std::mt19937::result_type>;
		
	public:

		static void newSeed();
		
		static int Int(int low = 0, int high = 10);
		static unsigned Unsigned(unsigned low = 0u, unsigned high = 10u);
		static float Float(float low = 0.0f, float high = 1.0f);
		static double Double(double low = 0.0, double high = 1.0);

	private:

		static MTEngine _randomEngine;
		static IntDistribution _intDistribution;
		
	};
	
}