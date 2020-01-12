#ifndef SQUARE_ARMY
#define SQUARE_ARMY


#include <vector>
#include "gen_random.h"
#include <iostream>
#include  <math.h>
#include "color_utils.h"
#include "algorithm"
#include <GLFW/glfw3.h>
//class SquareLink {
//};

class SquareArmy;

class Link {
public:
	Link(const Link&);
public:
	Link(SquareArmy& square_army_link); 
	SquareArmy& square_army_link;
	float hue_dif;
	//float hue_velocity_dif;

	float sat_dif;
	//sat_velocity_dif;

	float light_dif;
	//float light_velocity_dif;

	//float force_mag = 0;

	//float hue_force = 0;
	//float sat_force = 0;
	//float light_force = 0;

};

class SquareArmy {
public:


	GenRand &gen_rand;

	int x;
	int y;
	int hue_index;

	float& hue;
	float& sat;
	float& light;

	//float og_hue;
	//float og_sat;
	//float og_light;

	bool boundary;

	//float hue_change=0;

	float hue_velocity=0;
	float hue_acceleration=0;
	float hue_resistance;
	float hue_force = 0;
	float hue_mass;

	float light_velocity = 0;
	float light_acceleration = 0;
	float light_resistance;
	float light_force = 0;
	float light_mass;

	float sat_velocity = 0;
	float sat_acceleration = 0;
	float sat_resistance;
	float sat_force = 0;
	float sat_mass;

	std::vector<float*> target_list;
	int target_index = 1;

	float target_hue;
	float target_light;
	float target_sat;

	std::vector<Link> links;

	bool at_target = false;
	double cycle_start_time = glfwGetTime()-10;

public:
	SquareArmy(int t_x, int t_y, float& t_hue, float& t_sat, float& t_light, std::vector<float*> t_target_list,  GenRand& t_gen_rand, bool t_boundary, int hue_index);


	void update_target();

	void update_links_wave();
	void update_squares_wave();

	void make_link(SquareArmy& square_army);


};


#endif