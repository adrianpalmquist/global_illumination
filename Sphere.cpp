//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include "Sphere.h"

Sphere::Sphere(vec3 _position, float _radius, BaseMaterial* _material): position(_position), radius(_radius), material(_material) {
    radius2 = radius*radius; // optimized for ray intersection
}

float sum(vec3 vec) {
    return vec.x + vec.y + vec.z;
}

bool Sphere::RayIntersection(vec3 origin, vec3 direction, vec3 &intersection_point, vec3 &normal) {
    float t0, t1;

    vec3 L = origin - position;
    float a = dot(direction,direction);
    float b = 2 * dot(direction,L);
    float c = dot(L,L) - radius2;
    if (!solveQuadratic(a, b, c, t0, t1)) return false;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead
        if (t0 < 0) return false; // both t0 and t1 are negative
    }

    float t = t0;

    intersection_point = origin + direction * t;
    normal = normalize(intersection_point - position);

    return true;
}

bool Sphere::solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = (float) (- 0.5 * b / a);
    else {
        float q = (float) ((b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr)));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);

    return true;
}

vec3 Sphere::get_position() {
    return position;
}

float Sphere::get_radius() {
    return radius;
}

BaseMaterial * Sphere::get_material() {
    return material;
}