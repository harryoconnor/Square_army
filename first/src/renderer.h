#ifndef RENDERER
#define RENDERER

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
#include <vector>
#include <cmath>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;

};

class Renderer
{
private:


	int square_size;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int x_squares;
	int y_squares;
	int square_array_size;

	//void* buffptr; faster without?

	GLFWwindow* window;

	float positions[8] = {
		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f,

	};
	unsigned int indices[6] = {
		0,1,2,
		2,3,1
	};


	unsigned int vao;
	unsigned int buffer;
	unsigned int ibo;
	unsigned int ssbo;

	ShaderProgramSource source;

	unsigned int shader;

	int square_length_location;
	int square_y_squares_location;

public:
	int update_count = 0;
	double BufferData_time = 0;
	double glClear_time = 0;
	double glDrawElements_time = 0;
	double glfwSwapBuffers_time = 0;
	double glfwPollEvents_time = 0;

	Renderer(int t_square_size, int t_SCREEN_WIDTH, int t_SCREEN_HEIGHT);

	bool init();


	void GLClearError();
	bool GLLogCall(const char* function, const char* file, int line);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);


	void update(float* data);


	bool myglfwWindowShouldClose();

	void clean_up();


	void merge_shader_extension(std::string shader_file_name, std::string extension_file_name, std::string output_file_name);
};

#endif