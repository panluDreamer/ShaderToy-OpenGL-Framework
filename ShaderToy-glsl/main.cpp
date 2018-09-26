#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include<iostream>
#include<time.h>
#include <assert.h>
#include <glew.h>
#include<GLFW\glfw3.h>
#include <gli/gli.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.h"
#include "Shader.h"
using namespace std;
using glm::vec2;
using glm::vec3;
using glm::vec4;

GLFWwindow* window;
#define WIDTH 800
#define HEIGHT 600


void init_glfw_glew();

void init_glfw_glew() {
	// Initialize GLFW
	if (!glfwInit()){
		cout << "Failed to initialize GLFW" << endl;
		exit(-1);
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WIDTH, HEIGHT, "ShaderToy", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to open GLFW window.If you have an intel GPU, may be they are not 3.3 compatible" << endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		cout << "Failed to initialize GLEW" << endl;
		exit(-1);
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
	GLEW_ARB_debug_output;
}
int main(int argc, char **argv)
{
	cout << "init opengl and window context....." << endl;
	init_glfw_glew();
	cout << "init success" << endl;
	glClearColor(0.4f, 0.8f, 0.6f, 0.0f);
	Model quad;
	quad.init("quad.obj", false, true, false, false);
	Shader shader;
	shader.init("shader/main_vert.glsl", "shader/fire_ball_frag.glsl");
	//shader.init("shader/main_vert.glsl", "shader/unreal_intro_frag.glsl");
	vec3 iResolution = vec3(WIDTH, HEIGHT, 0);
	clock_t start_time = clock();
	clock_t curr_time;
	float playtime_in_second = 0;
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0) {
		glClear(GL_COLOR_BUFFER_BIT);
		curr_time = clock();
		playtime_in_second = (curr_time - start_time)*1.0f / 1000.0f;
		//cout << "playtime_in_second = " << playtime_in_second << endl;
		shader.use();
		shader.bind_vec3("iResolution", iResolution);
		shader.bind_float("iTime",playtime_in_second);
		quad.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}