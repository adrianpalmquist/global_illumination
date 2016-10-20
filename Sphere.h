//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_SPHERE_H
#define TNCG15_PROJ_SPHERE_H

#include "material/BaseMaterial.h"

using namespace glm;

class Sphere {
public:
    Sphere(vec3 _position, float _radius, BaseMaterial* _material);

    bool RayIntersection(vec3 origin, vec3 direction, vec3 &intersection_point, vec3 &normal);

    // Getters and setters
    vec3 get_position();
    float get_radius();
    BaseMaterial* get_material();
    bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);
private:
    vec3 position;
    float radius;
    float radius2; // radius²
    BaseMaterial* material;
};


#endif //TNCG15_PROJ_SPHERE_H
