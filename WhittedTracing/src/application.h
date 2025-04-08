#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include "context.h"
#include "common.h"

#include "film_plane.h"
#include "tracer_v1.h"
#include "./gl/gl.h"

class Application{
public:
	Application(){}
	
	int init(Context& context);
	void run(Context& context);
	void cleanup(Context& context);
};

int Application::init(Context& context){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	context.window = glfwCreateWindow(context.windowWidth, context.windowHeight, context.windowName.c_str(), nullptr, nullptr);
	if(context.window == nullptr){
		std::cout << "Window could not be created. " << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(context.window);
	glfwSetInputMode(context.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        	std::cout << "Failed to initialize GLAD. " << std::endl;
        	return -1;
    	}		

	context.mainShader = Shader("./src/gl/shaders/tVertex.vs", "./src/gl/shaders/tFragment.fs", true);
	context.screenTexture = Texturef(800, 600);
	context.screenTexture.data = (float*)malloc(800 * 600 * 4 * sizeof(float)); //!!!
        context.primaryRays.resize(480000);
	
	return 0;
}

void Application::run(Context& context){
	//Call simulation functions here
	initObjects(context);
	initFilm(context);
	createImage(context);
	initGraphics(context);

while (!glfwWindowShouldClose(context.window)){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.227f, 0.4f, 0.6f, 1.0f);
	
	glUseProgram(context.mainShader.ID);
	glBindTexture(GL_TEXTURE_2D, context.screenTexture.ID);
	glUniform1i(glGetUniformLocation(context.screenTexture.ID, "ourTexture"), 0);

	glBindVertexArray(context.VAO);
	glDrawArrays(GL_TRIANGLES, 0, context.vertices.size() / 5);
	glfwSwapBuffers(context.window);
        glfwPollEvents();
}

}

void Application::cleanup(Context& context){

}

#endif