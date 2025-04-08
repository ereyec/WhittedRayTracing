#ifndef TRACERV1_H
#define TRACERV1_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common.h"
#include "context.h"

//INPUT: a ray, 1 sphere
//DEPENDENCIES: glm, vector
//REF: Ray& ray

std::optional<IntersectionCandidate> t_intersection(Context& context, Ray& ray) {
    bool primaryIntersectionFound = false;
    std::vector<IntersectionCandidate> t_candidates;
    for (int i = 0; i < context.spheres.size(); i++) {
        Sphere& sphere = context.spheres[i];

        glm::vec3 oc = ray.origin - sphere.position;
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(ray.direction, oc);
        float c = glm::dot(oc, oc) - sphere.radius * sphere.radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant > 0) {
            primaryIntersectionFound = true;
            float t0 = (-b + std::sqrt(discriminant)) / (2 * a);
            float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
            float t_min = std::min(t0, t1);

            IntersectionCandidate intCand = { t_min, sphere };
            t_candidates.push_back(intCand);
        }
    }
    if (primaryIntersectionFound) {
        std::sort(t_candidates.begin(), t_candidates.end(),
            [](const IntersectionCandidate& a, const IntersectionCandidate& b)
            { return a.t < b.t;  }
        );
        IntersectionCandidate intersection = t_candidates[0];
        return intersection;
    } else {
        return {};
    }
    
}

void traceSingleRay(Context& context, Ray& ray, int x, int y){
    //BLOCK0
    glm::vec3 lsDir = glm::vec3(0.0f, 0.707f, 0.707f); //light source direction ('reverse' incident)
    glm::vec3 lsPosition = glm::vec3(0.0f, -5.0f, 5.0f);
    glm::vec3 pixelIntensity = glm::vec3(0.0f, 0.0f, 0.0f);

    //BLOCK1 detour
    std::optional<IntersectionCandidate> intersectionC = t_intersection(context, ray);    

    //BLOCK1
    /*bool primaryIntersectionFound = false;
    std::vector<IntersectionCandidate> t_candidates;
    for (int i = 0; i < context.spheres.size(); i++) {
        Sphere& sphere = context.spheres[i];

        glm::vec3 oc = ray.origin - sphere.position;
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(ray.direction, oc);
        float c = glm::dot(oc, oc) - sphere.radius * sphere.radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant > 0) {
            primaryIntersectionFound = true;
            float t0 = (-b + std::sqrt(discriminant)) / (2 * a);
            float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
            float t_min = std::min(t0, t1);

            IntersectionCandidate intCand = { t_min, sphere };
            t_candidates.push_back(intCand);
        }
    }*/

    //BLOCK2 detour 2
    if (intersectionC.has_value()) {
        IntersectionCandidate intersection = intersectionC.value();
        glm::vec3 intersectionPoint = ray.origin + intersection.t * ray.direction;
        glm::vec3 normal = glm::normalize(intersectionPoint - intersection.sphere.position);
        glm::vec3 lsDir2 = glm::normalize(lsPosition - intersectionPoint);
        float lighting = std::max(glm::dot(normal, lsDir2), 0.0f);
        pixelIntensity += lighting * intersection.sphere.kd;

        //Find secondary intersection
        if (glm::length(intersection.sphere.ks) > 0) {
            //we need the reflected ray
            Ray reflectedRay;
            reflectedRay.origin = intersectionPoint + ray.direction * 0.01f;
            reflectedRay.direction = glm::normalize(glm::reflect(-ray.direction, normal));

            //EXPERIMENTAL 1 BEGIN detour
            std::optional<IntersectionCandidate> intersectionC2 = t_intersection(context, reflectedRay);
            if (intersectionC2.has_value()) {
                IntersectionCandidate intersection2 = intersectionC2.value();
                glm::vec3 intersectionPoint2 = reflectedRay.origin + intersection2.t * reflectedRay.direction;
                glm::vec3 normal2 = glm::normalize(intersectionPoint2 - intersection2.sphere.position);
                glm::vec3 lsDir2 = glm::normalize(lsPosition - intersectionPoint2);
                float lighting = std::max(glm::dot(normal2, lsDir2), 0.0f);
                pixelIntensity += lighting * intersection2.sphere.kd;
            }
            //EXPERIMENTAL 1 END detour
            

            //EXPERIMENTAL 1 BEGIN
            /*bool primaryIntersectionFound2 = false;
            std::vector<IntersectionCandidate> t_candidates2;
            for (int i = 0; i < context.spheres.size(); i++) {
                Sphere& sphere = context.spheres[i];

                glm::vec3 oc = reflectedRay.origin - sphere.position;
                float a = glm::dot(reflectedRay.direction, reflectedRay.direction);
                float b = 2.0f * glm::dot(reflectedRay.direction, oc);
                float c = glm::dot(oc, oc) - sphere.radius * sphere.radius;
                float discriminant = b * b - 4 * a * c;

                if (discriminant > 0) {
                    primaryIntersectionFound2 = true;
                    float t0 = (-b + std::sqrt(discriminant)) / (2 * a);
                    float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
                    float t_min = std::min(t0, t1);

                    IntersectionCandidate intCand = { t_min, sphere };
                    t_candidates2.push_back(intCand);
                }
            }
            if (primaryIntersectionFound2) {
                std::sort(t_candidates2.begin(), t_candidates2.end(),
                    [](const IntersectionCandidate& a, const IntersectionCandidate& b)
                    { return a.t < b.t;  }
                );
                IntersectionCandidate intersection2 = t_candidates2[0];
                glm::vec3 intersectionPoint2 = reflectedRay.origin + intersection2.t * reflectedRay.direction;
                glm::vec3 normal2 = glm::normalize(intersectionPoint2 - intersection2.sphere.position);
                glm::vec3 lsDir2 = glm::normalize(lsPosition - intersectionPoint2);
                float lighting = std::max(glm::dot(normal2, lsDir2), 0.0f);
                pixelIntensity += lighting * intersection2.sphere.kd;
            }
            //EXPERIMENTAL 1 END*/
        }
    }

    //BLOCK2 detour
    /*if (intersectionC.has_value()) {
        IntersectionCandidate intersection = intersectionC.value();
        glm::vec3 intersectionPoint = ray.origin + intersection.t * ray.direction;
        glm::vec3 normal = glm::normalize(intersectionPoint - intersection.sphere.position);
        glm::vec3 lsDir2 = glm::normalize(lsPosition - intersectionPoint);
        float lighting = std::max(glm::dot(normal, lsDir2), 0.0f);
        pixelIntensity += lighting * intersection.sphere.kd;

        //Find secondary intersection
        if (glm::length(intersection.sphere.ks) > 0) {
            //we need the reflected ray
            Ray reflectedRay;
            reflectedRay.origin = intersectionPoint + ray.direction * 0.01f;
            reflectedRay.direction = glm::normalize(glm::reflect(-ray.direction, normal));

            //EXPERIMENTAL 1 BEGIN
            bool primaryIntersectionFound2 = false;
            std::vector<IntersectionCandidate> t_candidates2;
            for (int i = 0; i < context.spheres.size(); i++) {
                Sphere& sphere = context.spheres[i];

                glm::vec3 oc = reflectedRay.origin - sphere.position;
                float a = glm::dot(reflectedRay.direction, reflectedRay.direction);
                float b = 2.0f * glm::dot(reflectedRay.direction, oc);
                float c = glm::dot(oc, oc) - sphere.radius * sphere.radius;
                float discriminant = b * b - 4 * a * c;

                if (discriminant > 0) {
                    primaryIntersectionFound2 = true;
                    float t0 = (-b + std::sqrt(discriminant)) / (2 * a);
                    float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
                    float t_min = std::min(t0, t1);

                    IntersectionCandidate intCand = { t_min, sphere };
                    t_candidates2.push_back(intCand);
                }
            }
            if (primaryIntersectionFound2) {
                std::sort(t_candidates2.begin(), t_candidates2.end(),
                    [](const IntersectionCandidate& a, const IntersectionCandidate& b)
                    { return a.t < b.t;  }
                );
                IntersectionCandidate intersection2 = t_candidates2[0];
                glm::vec3 intersectionPoint2 = reflectedRay.origin + intersection2.t * reflectedRay.direction;
                glm::vec3 normal2 = glm::normalize(intersectionPoint2 - intersection2.sphere.position);
                glm::vec3 lsDir2 = glm::normalize(lsPosition - intersectionPoint2);
                float lighting = std::max(glm::dot(normal2, lsDir2), 0.0f);
                pixelIntensity += lighting * intersection2.sphere.kd;
            }

            //EXPERIMENTAL 1 END
        }
    }*/

    //BLOCK2
    /*if (primaryIntersectionFound) {
        std::sort(t_candidates.begin(), t_candidates.end(),
            [](const IntersectionCandidate& a, const IntersectionCandidate& b)
            { return a.t < b.t;  }
        );
        IntersectionCandidate intersection = t_candidates[0];
        glm::vec3 intersectionPoint = ray.origin + intersection.t * ray.direction;
        glm::vec3 normal = glm::normalize(intersectionPoint - intersection.sphere.position);
        glm::vec3 lsDir2 = glm::normalize(lsPosition - intersectionPoint);
        float lighting = std::max(glm::dot(normal, lsDir2), 0.0f);
        pixelIntensity += lighting * intersection.sphere.kd;

        //Find secondary intersection
        if (glm::length(intersection.sphere.ks) > 0) {
            //we need the reflected ray
            Ray reflectedRay;
            reflectedRay.origin = intersectionPoint + ray.direction * 0.01f;
            reflectedRay.direction = glm::normalize(glm::reflect(-ray.direction, normal));

            //EXPERIMENTAL 1 BEGIN
            bool primaryIntersectionFound2 = false;
            std::vector<IntersectionCandidate> t_candidates2;
            for (int i = 0; i < context.spheres.size(); i++) {
                Sphere& sphere = context.spheres[i];

                glm::vec3 oc = reflectedRay.origin - sphere.position;
                float a = glm::dot(reflectedRay.direction, reflectedRay.direction);
                float b = 2.0f * glm::dot(reflectedRay.direction, oc);
                float c = glm::dot(oc, oc) - sphere.radius * sphere.radius;
                float discriminant = b * b - 4 * a * c;

                if (discriminant > 0) {
                    primaryIntersectionFound2 = true;
                    float t0 = (-b + std::sqrt(discriminant)) / (2 * a);
                    float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
                    float t_min = std::min(t0, t1);

                    IntersectionCandidate intCand = { t_min, sphere };
                    t_candidates2.push_back(intCand);
                }
            }
            if (primaryIntersectionFound2) {
                std::sort(t_candidates2.begin(), t_candidates2.end(),
                    [](const IntersectionCandidate& a, const IntersectionCandidate& b)
                    { return a.t < b.t;  }
                );
                IntersectionCandidate intersection2 = t_candidates2[0];
                glm::vec3 intersectionPoint2 = reflectedRay.origin + intersection2.t * reflectedRay.direction;
                glm::vec3 normal2 = glm::normalize(intersectionPoint2 - intersection2.sphere.position);
                glm::vec3 lsDir2 = glm::normalize(lsPosition - intersectionPoint2);
                float lighting = std::max(glm::dot(normal2, lsDir2), 0.0f);
                pixelIntensity += lighting * intersection2.sphere.kd;
            }

            //EXPERIMENTAL 1 END


        }

    }*/
      
    int base_index = 4*(800*y + x);
    context.screenTexture.data[base_index] = pixelIntensity.x;
    context.screenTexture.data[base_index + 1] = pixelIntensity.y;
    context.screenTexture.data[base_index + 2] = pixelIntensity.z;
    context.screenTexture.data[base_index + 3] = 1.0f;
}

void createImage(Context& context){
    for(int x = 0; x < 800; x++){
        for(int y = 0; y < 600; y++){
            Ray& ray = context.primaryRays[800 * y + x];
            traceSingleRay(context, ray, x, y);	
        }
        std::cout << "Column (X): " << x << std::endl;      
    }

}

#endif