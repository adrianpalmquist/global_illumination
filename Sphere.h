//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_SPHERE_H
#define TNCG15_PROJ_SPHERE_H

#include "glm/glm.hpp"
#include "ColorDbl.h"

using namespace glm;

class Sphere {
public:
    Sphere(vec3 _position, float _radius, ColorDbl* _clr);

    bool RayIntersection(vec3 origin, vec3 direction, vec3 &intersection_point, vec3 &normal);

    // Getters and setters
    vec3 get_position();
    float get_radius();
    ColorDbl* get_clr();
private:
    vec3 position;
    float radius;
    ColorDbl* clr;
};


#endif //TNCG15_PROJ_SPHERE_H
