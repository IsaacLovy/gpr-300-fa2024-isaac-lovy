#include "PerlinNoise.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

ilgl::PerlinNoise::PerlinNoise(unsigned int seed)
{
	p.resize(256);

	std::iota(p.begin(), p.end(), 0);

	std::default_random_engine engine(seed);
	std::shuffle(p.begin(), p.end(), engine);

	p.insert(p.end(), p.begin(), p.end());
}
