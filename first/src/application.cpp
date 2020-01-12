# define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

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
#include "hsluv.h"
#include <experimental/filesystem>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
namespace fs = std::experimental::filesystem;

void rgb_array_to_hsl(uint8_t* rgb_image, float* hsl_image, int array_size) {
	for (int i = 0; i < array_size; i += 3) {
		double t_hue = 0;
		double t_sat = 0;
		double t_light = 0;
		rgb2hsluv(rgb_image[i] / 256.0, rgb_image[i + 1] / 256.0, rgb_image[i + 2] / 256.0, &t_hue, &t_sat, &t_light);
		hsl_image[i] = (float)t_hue;
		hsl_image[i + 1] = (float)t_sat;
		//write_data[i + 1] = 50;
		hsl_image[i + 2] = t_light;
	}

}




int main(void)
{
	std::vector<std::string> target_file_names;
	std::string path = "images";
	for (const auto& entry : fs::directory_iterator(path)) {
		target_file_names.push_back(entry.path().string());
	}

	int image_width, image_height, bpp;
	stbi_set_flip_vertically_on_load(true);
	uint8_t* rgb_image = stbi_load(target_file_names[0].c_str(), &image_width, &image_height, &bpp, 3);

	std::cout <<"width: "<< image_width<<" height: "<< image_height<<" bpp:"<< bpp << std::endl;
	std::cout <<"values:"<< std::endl;
	for (int i = 0; i < 100; i++) {
		std::cout << unsigned(rgb_image[i])<<" ";
	}
	std::cout << std::endl;




	




	unsigned int nthreads = std::thread::hardware_concurrency();
	std::cout << "thread count:" << nthreads<< std::endl; 
	//nthreads = 6;
	srand(time(NULL));
	int square_length = 2;
	int SCREEN_WIDTH = image_width * square_length;
	int SCREEN_HEIGHT = image_height* square_length;
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

	rgb_array_to_hsl(rgb_image, write_data, data_array_size);


	float*  target_data= new float[data_array_size];

	std::vector<float*> targets;

	for (int i = 0; i < target_file_names.size(); i++) {
		float* target_array = new float[data_array_size];
		int local_width, local_height, local_bpp;
		float*  local_target_data = new float[data_array_size];
		uint8_t* target_rgb_image = stbi_load(target_file_names[i].c_str(), &local_width, &local_height, &local_bpp, 3);


		rgb_array_to_hsl(target_rgb_image, local_target_data, data_array_size);
		targets.push_back(local_target_data);
		delete target_rgb_image;
	}

	
	read_data = write_data;

	SquareCollection all_squares(SCREEN_WIDTH, SCREEN_HEIGHT, square_length, write_data, targets, nthreads);
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