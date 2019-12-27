#ifndef SQUARE_ARMY
#define SQUARE_ARMY


#include <vector>
#include "gen_random.h"
#include <iostream>
#include  <math.h>
#include "color_utils.h"
//class SquareLink {
//};

class SquareArmy;

class Link {
public:
	Link(const Link&);
public:
	Link(SquareArmy& square_army_link); 
	SquareArmy& square_army_link;
	float hue_change = 0;
	float hue_change_change=0;


};

class SquareArmy {
public:


	GenRand &gen_rand;

	int x;
	int y;

	float& hue;
	float& sat;
	float& light;


	float hue_change=0;
	float hue_velocity=0;
	float hue_acceleration=0;

	std::vector<Link> links;

public:
	SquareArmy(int t_x, int t_y, float& t_hue, float& t_sat, float& t_light, GenRand& t_gen_rand);


	void update_links();
	void update_links_wave();
	void update_squares();
	void update_squares_wave();

	void make_link(SquareArmy& square_army);


};


#endif