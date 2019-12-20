#include "gen_random.h"

GenRand::GenRand(int seed) : e2(seed), standard_unif_100(0, 100), standard_unif_360(0, 360) {}

GenRand::GenRand() : e2(rand()), standard_unif_100(0, 100), standard_unif_360(0, 360) {}


float GenRand::getStandardUniform_100(){
	return standard_unif_100(e2);
}

float GenRand::getStandardUniform_360() {
	return standard_unif_360(e2);
}