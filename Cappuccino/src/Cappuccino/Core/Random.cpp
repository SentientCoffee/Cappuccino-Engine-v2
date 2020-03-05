#include "CappPCH.h"
#include "Cappuccino/Core/Random.h"

using namespace Capp;

Random::MTEngine Random::_randomEngine = MTEngine(std::random_device()());

void Random::newSeed() {
	_randomEngine.seed(std::random_device()());
}

int Random::Int(const int low, const int high) {
	const int range = static_cast<int>(_intDistribution(_randomEngine)) / static_cast<int>(std::numeric_limits<uint32_t>::max());
	return range * high + low;
}

unsigned Random::Unsigned(const unsigned low, const unsigned high) {
	const unsigned range = _intDistribution(_randomEngine) / std::numeric_limits<uint32_t>::max();
	return range * high + low;
}

float Random::Float(const float low, const float high) {
	const float range = static_cast<float>(_intDistribution(_randomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	return range * high + low;
}

double Random::Double(const double low, const double high) {
	const double range = static_cast<double>(_intDistribution(_randomEngine)) / static_cast<double>(std::numeric_limits<uint32_t>::max());
	return range * high + low;
}
