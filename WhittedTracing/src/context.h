#ifndef CONTEXT_H
#define CONTEXT_H

#include <iostream>
#include <vector>
#include "../include/engine/Shader.h"
#include "../include/engine/Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common.h"

struct Context{
	GLFWwindow* window = nullptr;	

	unsigned int windowWidth = 800;
	unsigned int windowHeight = 700;
	std::string windowName = "Whitted Ray Tracing";

	std::vector<float> vertices; //use: screen texture quad

	unsigned int VAO;
	unsigned int VBO;

	Texturef screenTexture;
	Shader mainShader;

	std::vector<Ray> primaryRays;
	std::vector<Sphere> spheres;
	
};

Context context;

#endif