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
#include "square_army.h"
#include <future>
#include "square_collection.h"


void ref_function_test(GenRand& reftest) {
	std::cout << "ref test:" << reftest.getStandardUniform_100() << std::endl;
}


int main(void)
{
	GenRand randtest(564);
	GenRand& reftest = randtest;
	std::cout << "random test:"<<randtest.getStandardUniform_100() << std::endl;
	std::cout << "ref test:"<<reftest.getStandardUniform_100() << std::endl;
	ref_function_test(reftest);
	unsigned int nthreads = std::thread::hardware_concurrency();
	std::cout << "thread count:" << nthreads<< std::endl; 
	//nthreads = 1;
	srand(time(NULL));
	int SCREEN_WIDTH = 1800;
	int SCREEN_HEIGHT = 1000;
	int square_length = 1;
	int x_squares = SCREEN_WIDTH / square_length;
	int y_squares = SCREEN_HEIGHT / square_length;
	int data_array_size = (x_squares * y_squares)*3;
	Renderer renderer(square_length, SCREEN_WIDTH, SCREEN_HEIGHT);
	renderer.init();
	double previousTime = glfwGetTime();
	int frameCount = 0;
	double fps_gap = 2.0;

	float* data = new float[data_array_size];
	SquareCollection test(SCREEN_WIDTH, SCREEN_HEIGHT, square_length, data, nthreads);

	//float* data = new float[square_array_size];
	//int square_index;
	//std::vector<SquareArmy> square_armies;
	//for (int y = 0; y < y_squares; y++) {
		//for (int x = 0; x < x_squares; x++) {
			//square_index = (y * x_squares*3) + x*3;
			//square_armies.push_back(SquareArmy(x, y, data[(square_index)], data[(square_index + 1)], data[(square_index + 2)]));
			//data[(square_index)] = hue;
			//data[(square_index + 1)] = sat;
			//data[(square_index + 2)] = light;
		//}
	//}



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
		//std::vector<std::future<void>> promise_list;
		//for (auto it = square_armies.begin(); it != square_armies.end(); ++it) {
			//it->update();#
			//promise_list.push_back(std::async(std::launch::async, &SquareArmy::update, &(*it)));
		//}
		//for (auto it = promise_list.begin(); it != promise_list.end(); ++it) {
			//it->wait();
		//}
		//promise_list.clear();
		//square_armies[5999].light = 0;
		test.update();
		renderer.update(data);
		
		//std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	renderer.clean_up();
	return 0;
}