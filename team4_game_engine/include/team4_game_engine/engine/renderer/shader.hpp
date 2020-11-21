#pragma once

#include <glad/glad.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace team4_game_engine::renderer {
	class Shader {
	public:
		uint32_t ID;

		Shader(const char* vertexPath, const char* fragmentPath);
		Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);
		~Shader();

		void Use();
		void Delete();

		// Read shader file
		std::string ReadShader(const char* shaderPath);
		// Compile shader source code
		unsigned int CompileShader(const char* shaderCode, int shaderType);

		//Update Uniforms functions
		void SetBool(const std::string& name, bool value);
		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void setMat4(const std::string& name, glm::mat4 value);
	};
}