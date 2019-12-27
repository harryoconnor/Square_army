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
#include <memory>




int main(void)
{

	
	unsigned int nthreads = std::thread::hardware_concurrency();
	std::cout << "thread count:" << nthreads<< std::endl; 
	//nthreads = 1000;
	srand(time(NULL));
	int SCREEN_WIDTH = 1000;
	int SCREEN_HEIGHT = 600;
	int square_length = 10;
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

	SquareCollection all_squares(SCREEN_WIDTH, SCREEN_HEIGHT, square_length, write_data, nthreads);
	memcpy(read_data, write_data, data_array_size * sizeof(float));
	//SquareArmy bob2 = all_squares.get_square_army(9, 5);
	//bob2.light = 0;

	
	int count = 0;
	std::future<void> renderer_promise;
	std::future<void> calc_promise;
	while (!renderer.myglfwWindowShouldClose())
	{
		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - previousTime >= fps_gap)
		{
			double fps = frameCount / fps_gap;
			std::cout << fps << std::endl;

			frameCount = 0;
			previousTime = currentTime;
		}
		
		calc_promise = std::async(std::launch::async, &SquareCollection::update, &all_squares);

		renderer.update(read_data);
		calc_promise.get();

		memcpy(read_data, write_data, data_array_size * sizeof(float));

		//std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	

	std::cout << "BufferData_time micro:" <<1000000* renderer.BufferData_time / renderer.update_count << std::endl;
	std::cout << "glClear_time micro:" << 1000000 * renderer.glClear_time / renderer.update_count << std::endl;
	std::cout << "glDrawElements_time micro:" << 1000000 * renderer.glDrawElements_time / renderer.update_count << std::endl;
	std::cout << "glfwSwapBuffers_time micro:" << 1000000 * renderer.glfwSwapBuffers_time / renderer.update_count << std::endl;
	std::cout << "glfwPollEvents_time micro:" << 1000000 * renderer.glfwPollEvents_time / renderer.update_count << std::endl;
	renderer.clean_up();
	return 0;
}