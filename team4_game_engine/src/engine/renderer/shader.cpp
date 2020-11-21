#include <team4_game_engine/engine/renderer/shader.hpp>

namespace team4_game_engine::renderer {
	Shader::Shader(const char* vertexPath, const char* fragmentPath) {
		unsigned int vertex = CompileShader(ReadShader(vertexPath).c_str(), GL_VERTEX_SHADER);
		unsigned int fragment = CompileShader(ReadShader(fragmentPath).c_str(), GL_FRAGMENT_SHADER);


		// Setup shader program
		ID = glCreateProgram();

		// Attach each shader
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);

		// Link Program to GPU
		glLinkProgram(ID);

		// check program result success
		int success;
		char infoLog[512];
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "Error: failed linking shader program\n" << infoLog << std::endl;
		}

		// Delete Shader after program setup finish
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
	{
		unsigned int vertex = CompileShader(ReadShader(vertexPath).c_str(), GL_VERTEX_SHADER);
		unsigned int geometry = CompileShader(ReadShader(geometryPath).c_str(), GL_VERTEX_SHADER);
		unsigned int fragment = CompileShader(ReadShader(fragmentPath).c_str(), GL_FRAGMENT_SHADER);

		ID = glCreateProgram();

		glAttachShader(ID, vertex);
		glAttachShader(ID, geometry);
		glAttachShader(ID, fragment);

		glLinkProgram(ID);

		int success;
		char infoLog[512];
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "Error: failed linking shader program\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(geometry);
		glDeleteShader(fragment);
	}

	std::string Shader::ReadShader(const char* shaderPath) {
		std::string sourceCode;
		std::ifstream shaderFile;

		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			shaderFile.open(shaderPath);

			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();

			shaderFile.close();

			sourceCode = shaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "Error: failed read on vertex shader file " << shaderPath << std::endl;
		}
		return  sourceCode;
	}

	unsigned int Shader::CompileShader(const char* ShaderCode, int shaderType) {
		unsigned int shader; // shader gpu id
		//Create shader on GPU
		shader = glCreateShader(shaderType);

		// Link source code
		glShaderSource(shader, 1, &ShaderCode, nullptr);

		// Compile source code
		glCompileShader(shader);

		// Check Success
		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "Error: failed compilation on shader\n" << infoLog << std::endl;
		}
		return shader; // Return shader gpu id
	}

	Shader::~Shader() {
		glDeleteProgram(ID);
	}

	void Shader::Use() {
		glUseProgram(ID);
	};

	void Shader::Delete() {
		glDeleteProgram(ID);
	}

	void Shader::SetBool(const std::string& name, bool value) {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void Shader::SetInt(const std::string& name, int value) {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void Shader::SetFloat(const std::string& name, float value) {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void Shader::setMat4(const std::string& name, glm::mat4 value) {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
}