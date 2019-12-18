#shader vertex
#version 430 core

layout(location = 0) in vec4 position;
void main()
{
	gl_Position = position;
};





#shader fragment
#version 430 core

layout(std430, binding = 3) buffer layoutName
{
	int data_SSBO[];
}my_data;

layout(location = 0) out vec4 color;
in vec4 gl_FragCoord;
//uniform int u_x_array_gradient;
//uniform float u_y_array_gradient;
uniform int u_square_length;
uniform int u_y_squares;

uniform vec3 u_test[10];
void main()
{
	//color = u_Color;

	//gl_FragCoord
	//float my_red;
	//float my_green;
	//float my_blue;
	//int length = 4;
	//int y_max = 1000;
	//int x_max = 500;
	
	//int y_squares = y_max/ length;
	//int x_squares = x_max / length;
	//&& gl_FragCoord.y>50
	int x = int(gl_FragCoord.x);
	int y = int(gl_FragCoord.y);
	//int red_index = ((y / length) * y_squares)*3 + (x / length)*3; 
	int square_index = ((y / u_square_length) * u_y_squares) + (x / u_square_length);
	//int red_index = ((y / length) * y_squares) * 3 + x/u_x_array_gradient;
	//int green_index = red_index + 1;
	//int blue_index = red_index + 2;

	vec4 color_min_alpha = unpackUnorm4x8(my_data.data_SSBO[square_index]);

	color = vec4(color_min_alpha.x, color_min_alpha.y, color_min_alpha.z, 1.0f);


	//my_red = my_data.data_SSBO[red_index];
	//my_green = my_data.data_SSBO[green_index];
	//my_blue = my_data.data_SSBO[blue_index];

	
	//uint red = 50;

	//color = vec4(my_red, my_green, my_blue, 1.0f);
	//color = vec4((my_data.data_SSBO[red_index]/255.0), my_data.data_SSBO[red_index+1] / 255.0, my_data.data_SSBO[red_index+2] / 255.0, 1.0f);
	//color = vec4(red, red, red, red);
	//color = vec4(test.x, test.y, test.z, 1.0f);
};
