//
// Created by Filip Kantedal on 06/09/16.
//
#pragma once

#include "ColorDbl.h"
#include "Scene.h"

class Ray {
public:
    static const int MAX_ITERATIONS = 5;

    Ray(vec3 *_start_point, vec3 *_end_point, int _ray_iteration);
    ~Ray();

    void TraceRay(Scene* scene);

    int get_ray_iterations();
private:
    vec3 * start_point;
    vec3 * end_point;
    ColorDbl clr;
    int ray_iteration;

    Ray* reflected_ray;
    Ray* refracted_ray;
};


Ray::~Ray() {

}

void Ray::TraceRay(Scene *scene) {

}

Ray::Ray(vec3 *_start_point, vec3 *_end_point, int _ray_iteration) : start_point(_start_point), end_point(_end_point) {
    ray_iteration = _ray_iteration + 1;
}

int Ray::get_ray_iterations() {
    return ray_iteration;
}
