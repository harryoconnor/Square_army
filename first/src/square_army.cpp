#include "square_army.h"

SquareArmy::SquareArmy(int t_x, int t_y, float& t_hue, float& t_sat, float& t_light, GenRand& t_gen_rand)
	:x(t_x), y(t_y), hue(t_hue), sat(t_sat), light(t_light), gen_rand(t_gen_rand), hue_change(0)
{
	//hue = gen_rand.getStandardUniform_360();
	//sat = gen_rand.getStandardUniform_100();
	//light = gen_rand.getStandardUniform_100();
	light = 50;
	if (x<800)
		hue = 0;
	else
		hue = 180;
	sat = 50;
}

void SquareArmy::update() {
	//std::cout<< "square rand:"<< gen_rand.getStandardUniform_100()<<std::endl;
	hue_change = gen_rand.getStandardUniform_100() / 20;
	//hue_change = 1;
	//float light_change = -0.1f;
	//light += light_change;
	//for (int i = 0; i < 1000; i++) {
		//hue_change+= gen_rand.getStandardUniform_100() / 300;
	// }
	hue += hue_change;
}

