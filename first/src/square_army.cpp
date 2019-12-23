#include "square_army.h"

SquareArmy::SquareArmy(int t_x, int t_y, float& t_hue, float& t_sat, float& t_light, GenRand* t_gen_rand)
	:x(t_x), y(t_y), hue(t_hue), sat(t_sat), light(t_light), gen_rand(t_gen_rand), hue_change(0.0f)
{
	//hue = gen_rand.getStandardUniform_360();
	//sat = gen_rand.getStandardUniform_100();
	//light = gen_rand.getStandardUniform_100();
	light = 50;
	if (x<400)
		hue = 0;
	else
		hue = 180;
	sat = 50;
}

void SquareArmy::update() {
	//std::cout<< "square rand:"<< gen_rand.getStandardUniform_100()<<std::endl;
	hue_change = (2.5-gen_rand->getStandardUniform_100() / 20);
	//hue_change = 1;
	//float light_change = -0.1f;
	//light += light_change;
	//for (int i = 0; i < 1000; i++) {
		//hue_change+= gen_rand.getStandardUniform_100() / 300;
	// }
	if (hue_change > 100 || hue_change < -100) {
		__debugbreak();
	}
	hue += hue_change;
}

void SquareArmy::update_gen_rand(GenRand* t_gen_rand) {
	gen_rand = t_gen_rand;
}


SquareArmy& SquareArmy::operator = (const SquareArmy& old_obj) {
	x = old_obj.x;
	y= old_obj.y;
	hue=old_obj.hue;
	sat = old_obj.sat;
	light = old_obj.light;
	hue_change = old_obj.hue_change;
	return *this;
}

