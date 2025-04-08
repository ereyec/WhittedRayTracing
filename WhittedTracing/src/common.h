#ifndef COMMON_H
#define COMMON_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//All common functions, structs, and classes used throughout the entire program defined here.

struct Ray{
  glm::vec3 origin;
  glm::vec3 direction;
};

struct Sphere{
   glm::vec3 position;
   float radius;
   glm::vec3 kd; //diffuse rgb coefficients
   glm::vec3 ks; //reflection rgb coefficients: to use these these weight the reflected ray's incident light?
};

struct IntersectionCandidate {
    float t;
    Sphere sphere; //Generics to be done later.
};

void pushV3T2(std::vector<float>& vertices, glm::vec3 vec, glm::vec2 tex){
	vertices.push_back(vec.x);
        vertices.push_back(vec.y);
        vertices.push_back(vec.z);
        vertices.push_back(tex.x);
        vertices.push_back(tex.y);
}

#endif
