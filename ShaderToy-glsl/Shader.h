#ifndef SHADER_H
#define SHADER_H

#include <glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <set>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Shader{
public:
	Shader(){
	}

	~Shader(){
		glDeleteProgram(_program);
	}

	void init(const char* vert_prog_path, const char* frag_prog_path){
		_program = LoadShaders(vert_prog_path, frag_prog_path);
	}

	void use(){
		glUseProgram(_program);
	}

	void bind_mat4(const char* name, glm::mat4 const & mat){
		GLint loc = get_uniform_loc(name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
	}

	void bind_bool(const char* name, const bool b){
		bind_int(name, b);
	}

	void bind_int(const char* name, const int i){
		GLint loc = get_uniform_loc(name);
		glUniform1i(loc, i);
	}

	void bind_float(const char* name, const float f){
		GLint loc = get_uniform_loc(name);
		glUniform1f(loc, f);
	}

	void bind_vec2(const char* name, glm::vec2 const & vec){
		GLint loc = get_uniform_loc(name);
		glUniform2fv(loc, 1, &vec[0]);
	}

	void bind_vec3(const char* name, glm::vec3 const & vec){
		GLint loc = get_uniform_loc(name);
		glUniform3fv(loc, 1, &vec[0]);
	}

	void bind_vec4_array(const char* name, std::vector<glm::vec4> vec){
		GLint loc = get_uniform_loc(name);
		glUniform4fv(loc, (GLsizei)vec.size(), glm::value_ptr(vec[0]));
	}

	void bind_texture(const char* name, GLuint texture, GLuint id, GLenum texture_type = GL_TEXTURE_2D){
		glActiveTexture(GLenum(GL_TEXTURE0 + id));
		glBindTexture(texture_type, texture);
		GLuint loc = glGetUniformLocation(_program, name);
		glUniform1i(loc, id);
	}

	GLuint get_program(){
		return _program;
	}

private:
	static GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path){
		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the Vertex Shader code from the file
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
		if (VertexShaderStream.is_open()) {
			std::string Line = "";
			while (getline(VertexShaderStream, Line))
				VertexShaderCode += "\n" + Line;
			VertexShaderStream.close();
		}
		else {
			printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
			getchar();
			return 0;
		}

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
		if (FragmentShaderStream.is_open()) {
			std::string Line = "";
			while (getline(FragmentShaderStream, Line))
				FragmentShaderCode += "\n" + Line;
			FragmentShaderStream.close();
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile Vertex Shader
		//printf("Compiling shader : %s\n", vertex_file_path);
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			if (VertexShaderErrorMessage[0] != '\0')
				printf("Error when compiling vertex shader: %s\n%s\n", vertex_file_path, &VertexShaderErrorMessage[0]);
		}
		// Compile Fragment Shader
		//printf("Compiling shader : %s\n", fragment_file_path);
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			if (FragmentShaderErrorMessage[0] != '\0')
				printf("Error when compiling fragment shader: %s\n%s\n", fragment_file_path, &FragmentShaderErrorMessage[0]);
		}
		// Link the program
		//printf("Linking program\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			if (ProgramErrorMessage[0] != '\0')
				printf("Error when linking %s and %s\n%s\n", vertex_file_path, fragment_file_path, &ProgramErrorMessage[0]);
		}

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return ProgramID;
	}

private:

	static std::set<std::string> uniform_not_found;
	GLuint _program = 0;

	GLint get_uniform_loc(const char * name){
		GLint loc = glGetUniformLocation(_program, name);

		if (loc == -1)
		{
			//	std::cout << "uniform [" + string(name) + "] not found!" << std::endl;
#if DEBUG || _DEBUG
			if (uniform_not_found.find(name) == uniform_not_found.end())
			{
				Debug::LogError("uniform [" + std::string(name) + "] not found!");
				uniform_not_found.insert(name);
			}
#else
			//Debug::LogError("uniform [" + std::string(name) + "] not found!");
#endif // DEBUG


		}
		return loc;
	}


};

#endif