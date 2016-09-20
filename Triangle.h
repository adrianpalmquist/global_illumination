//
// Created by Filip Kantedal on 06/09/16.
//
#pragma once

#include <iostream>
#include "ColorDbl.h"
#include "glm/glm.hpp"

using namespace glm;

class Triangle {
public:
    Triangle();
    ~Triangle();
    Triangle(vec3* _v1, vec3* _v2, vec3* _v3, ColorDbl* _clr);
    bool rayIntersection(vec3 origin, vec3 direction, vec3 &intersection_point);

    // Class functions
    vec3 get_normal();
    vec3 get_reflection(vec3 direction, vec3 normal);
    ColorDbl* get_clr();
private:
    vec3* v0;
    vec3* v1;
    vec3* v2;
    vec3 normal;
    ColorDbl* clr;

    // Class functions
    void calculateNormal();
};

Triangle::Triangle() {}

Triangle::~Triangle() {
}


Triangle::Triangle(vec3* _v1, vec3* _v2, vec3* _v3, ColorDbl* _clr) : v0(_v1), v1(_v2), v2(_v3), clr(_clr) {
    calculateNormal();
}

// Calculates normal from vertices
void Triangle::calculateNormal() {
    vec3 baseVec1 = *v1 - *v0;
    vec3 baseVec2 = *v2 - *v0;
    vec3 new_normal = normalize(cross(baseVec1, baseVec2));

    std::cout << baseVec1.x << " " << baseVec1.y << std::endl;
    normal = new_normal;
}

// Returns normal direction
vec3 Triangle::get_normal() { return normal; }

bool Triangle::rayIntersection(vec3 origin, vec3 direction, vec3 &intersection_point) {
    static const float EPS = 0.0001;

    vec3 v0v1(*v1 - *v0);
    vec3 v0v2(*v2 - *v0);

    //Begin calculating determinant - also used to calculate u parameter
    vec3 P(cross(direction, v0v2));
    float det = dot(v0v1, P);

    if(det > -EPS && det < EPS) return false;
    float inv_det = 1.0f / det;

    //Distance from vertex1 to ray origin
    vec3 T = origin - *v0;
    float u = dot(T, P);
    if(u < 0.0f || u > det) return false;

    vec3 Q = cross(T, v0v1);

    float v = dot(direction, Q);
    if(v < 0.0f || u+v > det) return false;

    float t = dot(v0v2, Q);

    if(t > EPS) {
        intersection_point = origin + inv_det*t*direction;
        return true;
    }
    std::cout << "No hit" << std::endl;
    return false;
}

ColorDbl *Triangle::get_clr() {
    return clr;
}

vec3 Triangle::get_reflection(vec3 direction, vec3 normal) {
    // float angle = acos(dot(direction,normal));
    vec3 reflected = normalize(direction - 2 * dot(direction,normal) * normal);

    return reflected;
}




