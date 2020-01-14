#include "CappPCH.h"
#include "Cappuccino/Core/Random.h"

using namespace Capp;

Random::HRC::time_point Random::_timeNow;
Random::HRC::duration Random::_seed;
Random::DefaultRandomEngine Random::_generator;

int Random::Int(const int low, const int high) {
	_timeNow = std::chrono::high_resolution_clock::now();
	_seed = _timeNow.time_since_epoch();
	_generator = std::default_random_engine(static_cast<unsigned>(_seed.count()));

	const std::uniform_int_distribution distribution(low, high);
	return distribution(_generator);
}

float Random::Float(const float low, const float high) {
	_timeNow = std::chrono::high_resolution_clock::now();
	_seed = _timeNow.time_since_epoch();
	_generator = std::default_random_engine(static_cast<unsigned>(_seed.count()));

	const std::uniform_real_distribution<float> distribution(low, high);
	return distribution(_generator);
}

double Random::Double(const double low, const double high) {
	_timeNow = std::chrono::high_resolution_clock::now();
	_seed = _timeNow.time_since_epoch();
	_generator = std::default_random_engine(static_cast<unsigned>(_seed.count()));

	const std::uniform_real_distribution<double> distribution(low, high);
	return distribution(_generator);
}
