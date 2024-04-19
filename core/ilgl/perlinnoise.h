#pragma once

//Isaac Lovy
//    https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
namespace ilgl 
{
	class PerlinNoise
	{
		std::vector<int> p;

		public:
			PerlinNoise();
			PerlinNoise(unsigned int seed);
			~PerlinNoise();

			double noise(double x, double y, double z);

		private:
			double fade(double t);
			double lerp(double t);
			double grad(int hash, double x, double y, double z);
	};
}
