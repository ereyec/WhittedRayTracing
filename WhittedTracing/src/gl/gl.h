#ifndef GL_H
#define GL_H

#include "../common.h"
#include "../context.h"

//DEPENDENCIES: glm, glfw, glad, vector

void initGraphics(Context& context){

//Define mesh vertices
glm::vec3 v1 = glm::vec3(-1, 1, 0);
glm::vec3 v2 = glm::vec3(1, 1, 0);
glm::vec3 v3 = glm::vec3(-1, -1, 0);
glm::vec3 v4 = glm::vec3(1, -1, 0);
glm::vec2 t1 = glm::vec2(0, 1);
glm::vec2 t2 = glm::vec2(1, 1);
glm::vec2 t3 = glm::vec2(0, 0);
glm::vec2 t4 = glm::vec2(1, 0);

pushV3T2(context.vertices, v1, t3);
pushV3T2(context.vertices, v2, t4);
pushV3T2(context.vertices, v3, t1);
pushV3T2(context.vertices, v2, t4);
pushV3T2(context.vertices, v3, t1);
pushV3T2(context.vertices, v4, t2);

//VAO/VBO
glGenVertexArrays(1, &context.VAO);
glGenBuffers(1, &context.VBO);
glBindVertexArray(context.VAO);
glBindBuffer(GL_ARRAY_BUFFER, context.VBO);
glBufferData(GL_ARRAY_BUFFER, context.vertices.size() * sizeof(float), context.vertices.data(), GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);

//Texturef
glBindTexture(GL_TEXTURE_2D, context.screenTexture.ID);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 600, 0, GL_RGBA, GL_FLOAT, context.screenTexture.data);
glBindTexture(GL_TEXTURE_2D, 0);

}
#endif
