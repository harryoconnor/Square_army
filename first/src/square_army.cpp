#include "square_army.h"

SquareArmy::SquareArmy(int t_x, int t_y, float& t_hue, float& t_sat, float& t_light, GenRand* t_gen_rand)
	:x(t_x), y(t_y), hue(t_hue), sat(t_sat), light(t_light), gen_rand(t_gen_rand), hue_change(0.0f)
{
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
	hue = gen_rand->getStandardUniform_360();
	//hue = 0;
	sat = 70;
	//sat= gen_rand->getStandardUniform_100()/2+50;
}
/*
void SquareArmy::update() {
	//std::cout<< "square rand:"<< gen_rand.getStandardUniform_100()<<std::endl;
	hue_change = 0;
	hue_change += (2.5-gen_rand->getStandardUniform_100() / 20)*5;
	//hue_change = 1;
	//float light_change = -0.1f;
	//light += light_change;
	//for (int i = 0; i < 1000; i++) {
		//hue_change+= gen_rand.getStandardUniform_100() / 300;
	// }
	//if (hue_change > 100 || hue_change < -100) {
		//__debugbreak();
	//}
	for (auto it = links.begin(); it != links.end(); ++it) {
		hue_change += (*it)->hue_change;
	}
	hue += hue_change;
}
*/

void SquareArmy::update_links() {
	for (auto link_point_it = links.begin(); link_point_it != links.end(); ++link_point_it) {
		//(*link_point_it)->hue_change = 0.1;
		float hue_dif = hue_distance(hue, (((*link_point_it)->square_army_link)->hue));
		//float hue_dif((((*link_point_it)->square_army_link)->hue) - hue);
		if (fabsf(hue_dif) < 50) {
			(*link_point_it)->hue_change = hue_dif / 10;
		}
		//(*link_point_it)->hue_change = ((((*link_point_it)->square_army_link)->hue) - hue) / 500;

	}
}

void SquareArmy::update_squares() {
	hue_change = 0;
	hue_change += (2.5 - gen_rand->getStandardUniform_100() / 20) * 5;
	for (auto it = links.begin(); it != links.end(); ++it) {
		hue_change += (*it)->hue_change;
	}
	hue = add_hues(hue, hue_change);
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



Link::Link(std::shared_ptr<SquareArmy > t_square_army_link):square_army_link(t_square_army_link){}

std::shared_ptr<Link> SquareArmy::make_link(std::shared_ptr<SquareArmy> square_army) {
	std::shared_ptr<Link> link = std::make_shared<Link>(square_army);
	links.push_back(link);
	return link;
}
