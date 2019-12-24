#ifndef SQUARE_ARMY
#define SQUARE_ARMY


#include <vector>
#include "gen_random.h"
#include <iostream>
//class SquareLink {
//};

class SquareArmy;

class Link {
public:
	Link(std::shared_ptr<SquareArmy > square_army_link);  //add other_link after creation in Square Collection
	std::shared_ptr<SquareArmy > square_army_link=nullptr;
	std::shared_ptr<Link > other_link=nullptr;
	float hue_change = 0.1;

};

class SquareArmy {
public:
	//std::vector<SquareLink> links;

	GenRand* gen_rand;

	int x;
	int y;

	float& hue;
	float& sat;
	float& light;

	float hue_change;

	std::vector<std::shared_ptr<Link>> links;

public:
	SquareArmy(int t_x, int t_y, float& t_hue, float& t_sat, float& t_light, GenRand* t_gen_rand);
	//SquareArmy(int t_x, int t_y, float& t_hue, float& t_sat, float& t_light);
	void update();

	void update_links();
	void update_squares();

	void update_gen_rand(GenRand* t_gen_rand);
	std::shared_ptr<Link> make_link(std::shared_ptr<SquareArmy > square_army);

	SquareArmy& operator = (const SquareArmy& old_obj);

};


#endif