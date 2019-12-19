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

uniform int u_square_length;
uniform int u_array_width;

void main()
{
	
	int x = int(gl_FragCoord.x);
	int y = int(gl_FragCoord.y);
	//int square_index = ((y / 10) * 100) + (x / 10);
	int square_index = ((y / u_square_length) * u_array_width) + (x / u_square_length);
	

	vec4 color_min_alpha = unpackUnorm4x8(my_data.data_SSBO[square_index]);

	vec3 test = hsluvToRgb(100.0f, 50.0f, 50.0f);

	color= vec4(test.x, test.y, test.z, 1.0f);


	//color = vec4(color_min_alpha.x, color_min_alpha.y, color_min_alpha.z, 1.0f);



};
