#pragma once

#include "Core.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

class Shader {
public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {

		// Vertex
		std::ifstream vertexFile(vertexShaderPath);
		std::stringstream stream;
		if(vertexFile.is_open()){
			std::string str;
			while(vertexFile.good()){
				std::getline(vertexFile, str);
				stream << str << '\n';
			}
		}
		// std::cout << stream.str();
		char errorBuffer[1024];

		std::string vertexShaderSource = stream.str();
		const char* vertex_source = vertexShaderSource.c_str();
		u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertex_source, nullptr);

		glCompileShader(vertexShader);
		i32 status;
	 	i32 length;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

		if(!status) {
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
			ASSERT(length < 1024);
			glGetShaderInfoLog(vertexShader, 1024, &length, errorBuffer);
			std::cout << "Failed To Compile Vertex Shader: [" << errorBuffer << "]" << std::endl;
			ASSERT(false);
		}

		// Fragment
		stream.clear();
		stream.str("");
		std::ifstream fragmentFile(fragmentShaderPath);
		if(fragmentFile.is_open()){
			std::string str;
			while(fragmentFile.good()){
				std::getline(fragmentFile, str);
				stream << str << '\n';
			}
		}
		// std::cout << stream.str();

		std::string fragmentShaderSource = stream.str();
		const char* frag_source = fragmentShaderSource.c_str();
		u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &frag_source, nullptr);

		glCompileShader(fragmentShader);
		status;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);

		if(!status) {
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
			ASSERT(length < 1024);
			glGetShaderInfoLog(fragmentShader, 1024, &length, errorBuffer);
			std::cout << "Failed To Compile Fragment Shader: [" << errorBuffer << "]" << std::endl;
			ASSERT(false);
		}

		u32 program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if(!status) {
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			ASSERT(length < 1024);
			glGetProgramInfoLog(program, 1024, &length, errorBuffer);
			std::cout << "Failed To Link Program: [" << errorBuffer << "]" << std::endl;
			ASSERT(false);
		}

		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		m_Program = program;

	}
	void Bind() {
		glUseProgram(m_Program);
	}

	~Shader() {
		glDeleteProgram(m_Program);
	}
private:
	u32 m_Program;
};