#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h> 
#include <time.h>  
#include <stdint.h> /* Needed for uint32_t and uint8_t */
#include <chrono>
#include <thread>
#include "renderer.h"




uint32_t t_pack_3_into_1(uint8_t red, uint8_t green, uint8_t blue) {
	//blue = 50;
	//red = 50;
	//green = 50;
	return (red << 0) | (green << 8) | (blue << 16);
}




int main(void)
{
	std::cout << "test_line" << std::endl;
	int SCREEN_WIDTH = 1600;
	int SCREEN_HEIGHT = 1000;
	int square_length = 1;
	int x_squares = SCREEN_WIDTH / square_length;
	int y_squares = SCREEN_HEIGHT / square_length;
	int square_array_size = (x_squares * y_squares);
	Renderer renderer(square_length, SCREEN_WIDTH, SCREEN_HEIGHT);
	renderer.init();
	srand(time(NULL));
	double previousTime = glfwGetTime();
	int frameCount = 0;
	double fps_gap = 2.0;

	std::vector < std::vector<std::vector<uint8_t>>> data;
	for (int y = 0; y < y_squares; y++) {
		std::vector<std::vector<uint8_t>> temp_row;
		for (int x = 0; x < x_squares; x++) {
			std::vector<uint8_t> temp_rgb;
			temp_rgb.push_back(rand() % 256);
			temp_rgb.push_back(rand() % 256);
			temp_rgb.push_back(rand() % 256);
			temp_row.push_back(temp_rgb);
		}
		data.push_back(temp_row);
		temp_row.clear();
	}
	std::cout << "SIZE VECTOR: " << data[40].size() << std::endl;

	//while (!glfwWindowShouldClose(window))
	while (!renderer.myglfwWindowShouldClose())
	{
		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - previousTime >= fps_gap)
		{
			double fps = frameCount / fps_gap;
			// Display the frame count here any way you want.
			std::cout << fps << std::endl;

			frameCount = 0;
			previousTime = currentTime;
		}
		

		
		renderer.update(data);
	
		//std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	renderer.clean_up();
	return 0;
}