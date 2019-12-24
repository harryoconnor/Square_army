

#include "color_utils.h"


float hue_distance(float hue1, float hue2) {
	float result = hue2- hue1;
	if (result > 180) {
		result -= 360;
	}
	else if (result < -180) {
		result += 360;
	}
	return result;
}


float add_hues(float hue1, float hue2) {
	float result = hue2 + hue1;
	if (result > 360) {
		result -= 360;
	}
	else if (result < 0) {
		result += 360;
	}
	return result;
}
