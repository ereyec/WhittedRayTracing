#ifndef FILMPLANE_H
#define FILMPLANE_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common.h"
#include "context.h"


void initObjects(Context& context){
	Sphere sphere1 = {glm::vec3(0.0f, 0.0f, -5.f), 2.0f, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f)};
	Sphere sphere2 = {glm::vec3(5.0f, -5.0f, -7.f), 2.0f, glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f,0.5f,1.0f)};
	Sphere sphere3 = {glm::vec3(-5.0f, 1.0f, -6.f), 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f)};
	Sphere sphere4 = { glm::vec3(5.0f, 0.0f, -12.f), 2.0f, glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f,0.5f,1.0f) };
	context.spheres.push_back(sphere1);
	context.spheres.push_back(sphere2);
	context.spheres.push_back(sphere3);
	context.spheres.push_back(sphere4);
}

//PROCEDURE void initFilm(){}
//INPUT: film plane specifications
//DEPENDENCIES: vector, glm, 

void initFilm(Context& context){
//The camera is positioned at the origin, and the film plane is centered at (0,0,-5).
//The film plane position is the top left corner of the film plane. 
glm::vec3 filmPlanePos = glm::vec3(-5, 5, -5);
const int width = 800;
const int height = 600;

for(int x = 0; x < width; x++){
    for(int y = 0; y < height; y++){

        float x_pixel = filmPlanePos.x + x*10.0f/800.0f;
        float y_pixel = filmPlanePos.y - y*10.0f/600.0f;

        glm::vec3 direction = glm::normalize(glm::vec3(x_pixel, y_pixel, filmPlanePos.z));

        Ray primaryRay = {glm::vec3(0.0f, 0.0f , 0.0f), direction};

	context.primaryRays[800 * y + x] = primaryRay;
    }
}

}
//OUTPUT: container of primary rays
#endif