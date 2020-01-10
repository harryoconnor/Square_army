#include "square_army.h"

SquareArmy::SquareArmy(int t_x, int t_y, float& t_hue, float& t_sat, float& t_light, float t_target_hue, float t_target_sat, float t_target_light, GenRand& t_gen_rand, bool t_boundary)
	:x(t_x), y(t_y), hue(t_hue), sat(t_sat), light(t_light), gen_rand(t_gen_rand), boundary(t_boundary),hue_mass(900),hue_resistance(0), 
	sat_mass(900), sat_resistance(0), light_mass(900), light_resistance(0),og_hue(hue), og_sat(sat), og_light(light),
	target_hue(t_target_hue) , target_sat(t_target_sat), target_light(t_target_light)
{
	links.reserve(8);
	//hue = gen_rand.getStandardUniform_360();
	//sat = gen_rand.getStandardUniform_100();
	//light = gen_rand.getStandardUniform_100();
	//light = 50;
	//sat = 80;
	//hue = 50;
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
	//hue = gen_rand.getStandardUniform_360();
	//hue = 0;
	//sat = 80;
	//sat= gen_rand->getStandardUniform_100()/2+50;
}



void SquareArmy::update_links_wave() {
	for (auto link = links.begin(); link != links.end(); ++link) {

		//loat hue_dif = hue_distance(hue, ((link->square_army_link).hue));

		link->hue_dif = hue_distance(hue, ((link->square_army_link).hue));  //dif equals force
		//link->hue_dif = 0;
		//link->hue_velocity_dif = ((link->square_army_link).hue_velocity)- hue_velocity;

		link->sat_dif = ((link->square_army_link).sat)- sat;
		//link->sat_velocity_dif =((link->square_army_link).sat_velocity)- sat_velocity;

		link->light_dif = ((link->square_army_link).light)-light;
		//link->light_velocity_dif = ((link->square_army_link).light_velocity)- light_velocity;

		//float force_multi = sqrt(pow(link->sat_dif,2)+ pow(link->light_dif, 2));

		//link->hue_force = 1 * link->hue_dif;
		//link->hue_force = 0;
		//link->sat_force = 1* link->sat_dif;
		//link->light_force = 1* link->light_dif;
		
	}
}

void SquareArmy::update_squares_wave() {
	//hue_change = 0;
	//float hue_sum = 0;
	//hue_change += (2.5 - gen_rand.getStandardUniform_100() / 10);

	if (!boundary) {

		light_force = 0;
		sat_force = 0;
		hue_force = 0;

		for (auto it = links.begin(); it != links.end(); ++it) {


			hue_force += it->hue_dif;
			sat_force += it->sat_dif;
			light_force += it->light_dif;
		}

		double time = glfwGetTime();
		double mod_time = fmod(time,40);
		if (mod_time > 10 && mod_time < 20) {
			if (abs(hue - target_hue) < pow((mod_time-15)/2,2)) {
				hue = target_hue;
				hue_force = 0;
				hue_velocity = 0;
				//hue_mass -= 0.01;
			}
			if (abs(sat - target_sat) < pow((mod_time - 15) /3, 2)) {
				sat = target_sat;
				sat_force = 0;
				sat_velocity = 0;
				//sat_mass -= 0.01;
			}
			if (abs(light - target_light) < pow((mod_time - 15) /3, 2)) {
				light = target_light;
				light_force = 0;
				light_velocity = 0;
				//light_mass -= 0.01;
			}
		}
		else if (mod_time > 30) {
			if (abs(hue - og_hue) < pow((mod_time - 30), 2)) {
				hue = og_hue;
				hue_force = 0;
				hue_velocity = 0;
				//hue_mass -= 0.01;
			}
			if (abs(sat - og_sat) < pow((mod_time - 30) / 3, 2)) {
				sat = og_sat;
				sat_force = 0;
				sat_velocity = 0;
				//sat_mass -= 0.01;
			}
			if (abs(light - og_light) < pow((mod_time - 30) / 3, 2)) {
				light = og_light;
				light_force = 0;
				light_velocity = 0;
				//light_mass -= 0.01;
			}
		}




		hue_acceleration = hue_force / hue_mass;
		hue_velocity += hue_acceleration;
		hue = add_hues(hue, hue_velocity);
		hue_velocity = hue_velocity * (1 - hue_resistance);

		light_acceleration = light_force / light_mass;
		light_velocity += light_acceleration;
		light = light + light_velocity;
		light_velocity = light_velocity * (1 - light_resistance);



		sat_acceleration = sat_force / sat_mass;
		sat_velocity += sat_acceleration;
		sat = sat + sat_velocity;
		sat_velocity = sat_velocity * (1 - sat_resistance);



		sat = std::max(0.0f, std::min(sat, 100.0f));
		light = std::max(0.0f, std::min(light, 100.0f));


		//double time_mod = fmod(glfwGetTime(), 10);
	}


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