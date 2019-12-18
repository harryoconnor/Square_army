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
	Renderer(int t_square_size, int t_SCREEN_WIDTH, int t_SCREEN_HEIGHT);

	bool init();


	void GLClearError();
	bool GLLogCall(const char* function, const char* file, int line);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	void update(uint32_t* data);

	void update(std::vector < std::vector<std::vector<uint8_t>>>&data);

	bool myglfwWindowShouldClose();

	void clean_up();

	uint32_t pack_3_into_1(uint8_t red, uint8_t green, uint8_t blue);
};

#endif