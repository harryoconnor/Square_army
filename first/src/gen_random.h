#ifndef GEN_RANDOM
#define GEN_RANDOM

#include <random>

class GenRand
{
public:
	std::minstd_rand e2;
	std::uniform_real_distribution<float>standard_unif_100;
	std::uniform_real_distribution<float>standard_unif_360;

	GenRand(int seed);
	GenRand();
	float getStandardUniform_100();
	float getStandardUniform_360();
};
#endif