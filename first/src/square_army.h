#ifndef SQUARE_ARMY
#define SQUARE_ARMY


#include <vector>
#include "gen_random.h"
#include <iostream>
//class SquareLink {
//};


class SquareArmy {
public:
	//std::vector<SquareLink> links;

	GenRand& gen_rand;

	int x;
	int y;

	float& hue;
	float& sat;
	float& light;

	float hue_change;
public:
	SquareArmy(int t_x, int t_y, float& t_hue, float& t_sat, float& t_light, GenRand& t_gen_rand);
	//SquareArmy(int t_x, int t_y, float& t_hue, float& t_sat, float& t_light);
	void update();
	//void add_neighbor(SquareArmy* other_square);

};


#endif