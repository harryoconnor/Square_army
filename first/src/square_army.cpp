#include "square_army.h"

SquareArmy::SquareArmy(int t_x, int t_y, float& t_hue, float& t_sat, float& t_light, GenRand& t_gen_rand)
	:x(t_x), y(t_y), hue(t_hue), sat(t_sat), light(t_light), gen_rand(t_gen_rand), hue_change(0.0f)
{
	links.reserve(8);
	//hue = gen_rand.getStandardUniform_360();
	//sat = gen_rand.getStandardUniform_100();
	//light = gen_rand.getStandardUniform_100();
	light = 50;
	/*
	if (x < 100) {
		if (y < 60)
			hue = 0;
		else
			hue = 90;
	}else {
		if (y < 60)
			hue = 180;
		else
			hue = 270;
	}
	*/
	hue = gen_rand.getStandardUniform_360();
	//hue = 0;
	sat = 80;
	//sat= gen_rand->getStandardUniform_100()/2+50;
}


void SquareArmy::update_links() {
	for (auto link = links.begin(); link != links.end(); ++link) {
		float hue_dif = hue_distance(hue, ((link->square_army_link).hue));
		if (fabsf(hue_dif) < 50) {
			link->hue_change = hue_dif / 10;
		}

	}
}

void SquareArmy::update_squares() {
	hue_change = 0;
	//hue_change += (2.5 - gen_rand.getStandardUniform_100() / 20) * 5;
	for (auto it = links.begin(); it != links.end(); ++it) {
		hue_change += it->hue_change;
	}
	hue = add_hues(hue, hue_change);
}

void SquareArmy::update_links_wave() {
	for (auto link = links.begin(); link != links.end(); ++link) {

		float hue_dif = hue_distance(hue, ((link->square_army_link).hue));

		link->hue_change = hue_dif;

	}
}

void SquareArmy::update_squares_wave() {
	hue_change = 0;
	float hue_sum = 0;
	//hue_change += (2.5 - gen_rand.getStandardUniform_100() / 10);
	for (auto it = links.begin(); it != links.end(); ++it) {
		hue_sum += it->hue_change;
	}
	hue_acceleration = hue_sum / 400+ gen_rand.getStandardUniform_100()/10000;
	hue_velocity += hue_acceleration;
	hue_change = hue_velocity;
	hue = add_hues(hue, hue_change);
	hue_velocity *= 0.995;
}



Link::Link(SquareArmy& t_square_army_link):square_army_link(t_square_army_link){

}

void SquareArmy::make_link(SquareArmy& square_army) {
	links.emplace_back(square_army);
}


Link::Link(const Link& other): square_army_link(other.square_army_link) {
	std::cout << "dont copy link"<<std::endl;
}





/*
void SquareArmy::update_gen_rand(GenRand* t_gen_rand) {
	gen_rand = t_gen_rand;
}
*/

/*
SquareArmy& SquareArmy::operator = (const SquareArmy& old_obj) {
	x = old_obj.x;
	y= old_obj.y;
	hue=old_obj.hue;
	sat = old_obj.sat;
	light = old_obj.light;
	hue_change = old_obj.hue_change;
	return *this;
}
*/