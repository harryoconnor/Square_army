
#include"renderer.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))



Renderer::Renderer(int t_square_size, int t_SCREEN_WIDTH, int t_SCREEN_HEIGHT):square_size(t_square_size), SCREEN_WIDTH(t_SCREEN_WIDTH), SCREEN_HEIGHT(t_SCREEN_HEIGHT){

};


bool Renderer::init() {
	x_squares = SCREEN_WIDTH / square_size;
	y_squares = SCREEN_HEIGHT / square_size;
	square_array_size = (x_squares * y_squares);

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World2", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "glew bad" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;


	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &ssbo));
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo));
	GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, square_array_size * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo));

	source = ParseShader("res/shaders/basic.shader");
	std::cout << "VertexSource" << std::endl;
	std::cout << source.VertexSource << std::endl;
	std::cout << "FragmentSource" << std::endl;
	std::cout << source.FragmentSource << std::endl;
	shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	square_length_location = glGetUniformLocation(shader, "u_square_length");
	GLCall(glUniform1i(square_length_location, square_size));

	square_y_squares_location = glGetUniformLocation(shader, "u_array_width");
	GLCall(glUniform1i(square_y_squares_location, x_squares));


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

}

uint32_t Renderer::pack_3_into_1(uint8_t red, uint8_t green, uint8_t blue) {
	//blue = 250;
	//red = 0;
	//green = 0;
	return (red << 0) | (green << 8) | (blue << 16);
}

void Renderer::GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool Renderer::GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		//std::cout << "error message:" << gluErrorString(error);
		return false;
	}
	return true;
}


ShaderProgramSource Renderer::ParseShader(const std::string& filepath)
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

unsigned int Renderer::CompileShader(unsigned int type, const std::string& source) {
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
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Renderer::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

bool Renderer::myglfwWindowShouldClose() {
	return glfwWindowShouldClose(window);
}

void Renderer::update(uint32_t* data) {
	GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, square_array_size * sizeof(uint32_t), data, GL_DYNAMIC_DRAW));
	glClear(GL_COLOR_BUFFER_BIT);
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
	glfwSwapBuffers(window);
	glfwPollEvents();
}



void Renderer::update(std::vector < std::vector<std::vector<uint8_t>>> &data) {
	uint32_t* packed_data = new uint32_t[square_array_size];
	for (int y = 0; y <y_squares; ++y)
	{
		for (int x = 0; x < x_squares; ++x) {
			//std::vector<uint8_t> square_rgb = data[y][x];
			packed_data[(y * x_squares) + x] = pack_3_into_1(data[y][x][0], data[y][x][1], data[y][x][2]);
		}
	}
	update(packed_data);
	delete packed_data;
}



void Renderer::clean_up() {
	glDeleteProgram(shader);
	glfwTerminate();
}