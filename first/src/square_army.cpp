#include "square_army.h"

SquareArmy::SquareArmy(int t_x, int t_y, float& t_hue, float& t_sat, float& t_light, std::vector<float*> t_target_list, GenRand& t_gen_rand, bool t_boundary , int t_hue_index)
	:x(t_x), y(t_y), hue(t_hue), sat(t_sat), light(t_light), gen_rand(t_gen_rand), boundary(t_boundary),hue_mass(400),hue_resistance(0),
	sat_mass(400), sat_resistance(0), light_mass(400), light_resistance(0),
	target_list(t_target_list), hue_index(t_hue_index)
{
	links.reserve(8);
	update_target();
	//light = 50;
	//sat = 80;
	//hue = 50;
	
}

void SquareArmy::update_target() {
	target_hue = target_list[target_index][hue_index];
	target_sat = target_list[target_index][hue_index+1];
	target_light = target_list[target_index][hue_index+2];
	target_index++;
	if (target_index >= target_list.size() ) {
		target_index = 0;
	}
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
	boundary = false;
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
		//double time = 10;
		double cycle_time = time - cycle_start_time;

		if (at_target) {
			hue = target_hue;
			hue_force = 0;
			hue_velocity = 0;
			sat = target_sat;
			sat_force = 0;
			sat_velocity = 0;
			light = target_light;
			light_force = 0;
			light_velocity = 0;
			if (cycle_time > 10) {
				cycle_start_time = time;
				at_target = false;
				update_target();
			}
		}
		else {

			if (cycle_time > 10) {
				if (abs(hue - target_hue) + abs(sat - target_sat) + abs(light - target_light) < pow((cycle_time - 10) / 4, 2)) {
					bool all_three = true;
					//hue = target_hue;
					if (abs(hue - target_hue) < 20) {
						hue = target_hue;
						hue_force = 0;
						hue_velocity = 0;
					}
					else {
						hue_force = (target_hue - hue) * abs(target_hue - hue) / 50;
						all_three = false;
						//hue_resistance = 0.25 / (1 + abs(hue - target_hue));
					}

					if (abs(sat - target_sat) < 10) {
						sat = target_sat;
						sat_force = 0;
						sat_velocity = 0;

					}
					else {
						sat_force = (target_sat - sat) * abs(target_sat - sat) / 50;
						all_three = false;
						//sat_resistance = 0.25 / (1 + +abs(sat - target_sat));
					}
					//sat_velocity = 0;
					//light = target_light;
					//light_velocity = 0;
					if (abs(light - target_light) < 10) {
						light = target_light;
						light_force = 0;
						light_velocity = 0;
					}
					else {
						light_force = (target_light - light) * abs(target_light - light) / 50;
						all_three = false;
						//light_resistance = 0.25 / (1 + abs(light - target_light));
					}
					//at_target = true;
					//cycle_start_time = time;
					//hue_mass -= 0.01;

					if (all_three) {
						at_target = true;
						cycle_start_time = time;
					}
				}
				/*
				if (abs(sat - target_sat) < 1) {
					sat = target_sat;
					sat_force = 0;
					sat_velocity = 0;
					at_target = true;
					//sat_mass -= 0.01;
				}

				if (abs(light - target_light) < 1) {
					light = target_light;
					light_force = 0;
					light_velocity = 0;
					at_target = true;
					//light_mass -= 0.01;
				}

				if (at_target == true) {
					cycle_start_time = time;
				}
				*/
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


		if (sat > 100) {
			sat = 100;
			if (sat_velocity > 0) {
				sat_velocity = 0;
			}
		}
		else if (sat < 0){
			sat = 0;
			if (sat_velocity < 0) {
				sat_velocity = 0;
			}

		}
		if (light > 100) {
			light = 100;
			if (light_velocity > 0) {
				light_velocity = 0;
			}
		}
		else if (sat < 0) {
			light = 0;
			if (light_velocity < 0) {
				light_velocity = 0;
			}

		}



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