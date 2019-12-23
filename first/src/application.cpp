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



#define TIME_TEST


int main(void)
{

	
	unsigned int nthreads = std::thread::hardware_concurrency();
	std::cout << "thread count:" << nthreads<< std::endl; 
	//nthreads = 1;
	srand(time(NULL));
	int SCREEN_WIDTH = 1400;
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

	float* write_data = new float[data_array_size];
	float* read_data = new float[data_array_size];

	SquareCollection test(SCREEN_WIDTH, SCREEN_HEIGHT, square_length, write_data, nthreads);
	memcpy(read_data, write_data, data_array_size * sizeof(float));
	//SquareArmy& bob2 = test.get_square_army(9, 5);
	//bob2.light = 0;

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
	int count = 0;
	double before_time=0;
	double time_after_call=0;
	double total_render_time=0;
	double total_calc_time=0;
	std::future<void> renderer_promise;
	std::future<void> calc_promise;
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
		//count++; 
		//before_time = glfwGetTime();

		//test.update();
		calc_promise = std::async(std::launch::async, &SquareCollection::update, &test);
		//calc_promise.get();
		//time_after_call= glfwGetTime();
		//total_calc_time += (time_after_call - before_time);
		//before_time = glfwGetTime();
		//renderer_promise = std::async(std::launch::async, &Renderer::update, &renderer, read_data);
		renderer.update(read_data);
		calc_promise.get();
		//calc_promise.get();
		//renderer_promise.get();

		memcpy(read_data, write_data, data_array_size * sizeof(float));
		//renderer.update(read_data);
		//time_after_call = glfwGetTime();
		//total_render_time += (time_after_call - before_time);
		
		//std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	
	std::cout << "total_calc_time:" << 1000000 * total_calc_time / count << std::endl;
	std::cout << "total_render_time:" << 1000000 * total_render_time / count << std::endl << std::endl << std::endl;

	std::cout << "BufferData_time micro:" <<1000000* renderer.BufferData_time / renderer.update_count << std::endl;
	std::cout << "glClear_time micro:" << 1000000 * renderer.glClear_time / renderer.update_count << std::endl;
	std::cout << "glDrawElements_time micro:" << 1000000 * renderer.glDrawElements_time / renderer.update_count << std::endl;
	std::cout << "glfwSwapBuffers_time micro:" << 1000000 * renderer.glfwSwapBuffers_time / renderer.update_count << std::endl;
	std::cout << "glfwPollEvents_time micro:" << 1000000 * renderer.glfwPollEvents_time / renderer.update_count << std::endl;
	renderer.clean_up();
	return 0;
}