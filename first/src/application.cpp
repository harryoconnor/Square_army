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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



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
	int image_width, image_height, bpp;
	stbi_set_flip_vertically_on_load(true);
	//uint8_t* rgb_image = stbi_load("hue_contrast.jpg", &image_width, &image_height, &bpp, 3);
	//uint8_t* rgb_image = stbi_load("hue_contrast_large.jpg", &image_width, &image_height, &bpp, 3);
	//uint8_t* rgb_image = stbi_load("mona.jpg", &image_width, &image_height, &bpp, 3);
	//uint8_t* rgb_image = stbi_load("boris.jpg", &image_width, &image_height, &bpp, 3);
	uint8_t* rgb_image = stbi_load("boris_same.jpg", &image_width, &image_height, &bpp, 3);

	uint8_t* target_rgb_image = stbi_load("trump_same.jpg", &image_width, &image_height, &bpp, 3);

	//uint8_t* rgb_image = stbi_load("bt_resize.jpg", &image_width, &image_height, &bpp, 3);
	std::cout <<"values:"<< std::endl;
	for (int i = 0; i < 100; i++) {
		std::cout << unsigned(rgb_image[i])<<" ";
	}
	std::cout << std::endl;


	//std::cout << "bpp is:" << bpp << std::endl;


	double hue=0;
	double sat=0;
	double light=0;
	double red=0.8;
	double green=0.8;
	double blue=0.8;

	//rgb2hsluv(red, green, blue, &hue, &sat, &light);
	//std::cout << "hue is: " << hue << " sat is: " << sat << " light is : " << light << std::endl;
	//hsluv2rgb(hue, sat, light, &red, &green, &blue);
	//std::cout << "red is: " << red << " green is: " << green << " blue is : " << blue << std::endl;


	unsigned int nthreads = std::thread::hardware_concurrency();
	std::cout << "thread count:" << nthreads<< std::endl; 
	//nthreads = 1000;
	srand(time(NULL));
	int square_length = 2;
	int SCREEN_WIDTH = image_width* square_length;
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
	rgb_array_to_hsl(target_rgb_image, target_data, data_array_size);

	
	read_data = write_data;

	SquareCollection all_squares(SCREEN_WIDTH, SCREEN_HEIGHT, square_length, write_data, target_data, nthreads);
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