#include "Magma/Utility/Random.h"

namespace mg
{
	std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;
	unsigned int Random::s_Seed;
}