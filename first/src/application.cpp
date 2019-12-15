#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h> 
#include <time.h>  

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function<<
			" "<<file<< ":"<< line <<std::endl;
		return false;
	}
	return true;
}

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;

};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return{ ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile"<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")<<" shader" <<std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
int SCREEN_WIDTH = 1000;
int SCREEN_HEIGHT = 600;


int main(void)
{
	int square_size = 1;

	int x_squares = SCREEN_WIDTH / square_size;
	int y_squares = SCREEN_HEIGHT / square_size;
	//int square_array_size = 500000;
	int square_array_size = (x_squares * y_squares * 3);

	srand(time(NULL));
	double previousTime = glfwGetTime();
	int frameCount = 0;
	double fps_gap = 2.0;
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World2", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "glew bad"<<std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] = {
		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f,

	};
	unsigned int indices[] = {
		0,1,2,
		2,3,1
	};

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 *2* sizeof(float), positions, GL_STATIC_DRAW));
	
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW));



	float* data = new float[square_array_size];
	for (int i = 0; i < square_array_size; i++) {
		float r;
		if (i % 3 == 0) {
			r = 90;
			r = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		}
		else {
			r = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		}
		data[i] = r;
	}

	unsigned int ssbo;
	GLCall(glGenBuffers(1, &ssbo));
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo));
	GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, square_array_size * sizeof(float), data, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo));
	//GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

	ShaderProgramSource source = ParseShader("res/shaders/basic.shader");
	std::cout << "VertexSource" << std::endl;
	std::cout << source.VertexSource << std::endl;
	std::cout << "FragmentSource" << std::endl;
	std::cout << source.FragmentSource << std::endl;
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	//unsigned int ssbo_index = 0;
	//ssbo_index = glGetProgramResourceIndex(shader, GL_SHADER_STORAGE_BLOCK, "data");
	//unsigned int ssbo_binding_point_index = 3;
	//glShaderStorageBlockBinding(shader, ssbo_index, ssbo_binding_point_index);


	//int location = glGetUniformLocation(shader, "u_square_color");
	int square_length_location = glGetUniformLocation(shader, "u_square_length");
	ASSERT(square_length_location != -1);
	float red = 0.0f;

	//GLCall(glUniform4f(location, red, 0.3f, 0.8f, 1.0f));
	//int x_array_gradient = 3 / square_size;
	//int y_array_gradient = (3*y_squares)/ square_size;

	GLCall(glUniform1i(square_length_location, square_size));

	int square_y_squares_location = glGetUniformLocation(shader, "u_y_squares");
	ASSERT(square_y_squares_location != -1);
	GLCall(glUniform1i(square_y_squares_location, y_squares));

	//GLCall(glUniform3fv(location, 10, test));
	/* Loop until the user closes the window */
	int max_uniform_size;
	int max_block_size;
	int max_block_number;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &max_uniform_size);
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &max_block_size);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &max_block_number);
	std::cout << "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS: " << max_uniform_size << std::endl;
	std::cout << "GL_MAX_UNIFORM_BLOCK_SIZE: " << max_block_size << std::endl;
	std::cout << "GL_MAX_UNIFORM_BLOCK_SIZE: " << GL_MAX_UNIFORM_BLOCK_SIZE << std::endl;
	std::cout << "GL_MAX_FRAGMENT_UNIFORM_BLOCKS: " << max_block_number << std::endl;


	while (!glfwWindowShouldClose(window)) 
	{
		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - previousTime >= fps_gap)
		{
			double fps = frameCount / fps_gap;
			// Display the frame count here any way you want.
			std:: cout << fps << std::endl;

			frameCount = 0;
			previousTime = currentTime;
		}
		red += 0.01f;
		if (red > 1) {
			red = 0;
		}
		for (int i = 0; i < square_array_size; i++) {
			float r;
			if (i % 3 == 0) {
				r = 90;
				r = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			}
			else {
				r = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			}
			data[i] = r;
		}
		//test[0] = red;
		data[0] = red;
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, square_array_size *sizeof(float), data, GL_DYNAMIC_DRAW));
		//memcpy(p, data, 50 * sizeof(float));
		//GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, 50 * sizeof(float), data, GL_DYNAMIC_DRAW));
		//GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo));
		//GLCall(glUniform4f(location, red, 0.3f, 0.8f, 1.0f));
		//GLCall(glUniform1fv(location, 10, test));
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr ));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}