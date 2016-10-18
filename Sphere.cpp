//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include "Sphere.h"

Sphere::Sphere(vec3 _position, float _radius, BaseMaterial* _material): position(_position), radius(_radius), material(_material) {}

float sum(vec3 vec) {
    return vec.x + vec.y + vec.z;
}

bool Sphere::RayIntersection(vec3 origin, vec3 direction, vec3 &intersection_point, vec3 &normal) {
    vec3 op = position - origin;
    float t, epsilon = 0.0001f;
    float b = dot(op, direction);
    float disc = b * b - dot(op, op) + radius * radius;

    if (disc < 0) {
        return false;
    }

    disc = sqrtf(disc);

    t = b - disc;
    if (t > epsilon) {
        intersection_point = origin + direction * t;
        normal = normalize(intersection_point - position);
        //normal = dot(normal, direction) < 0 ? normal : -1.0f * normal;
        return true;
    }

    t = b + disc;
    if (t > epsilon) {
        intersection_point = origin + direction * t;
        normal = normalize(intersection_point - position);
        //normal = dot(normal, direction) < 0 ? normal : -1.0f * normal;
        return true;
    }

    return false;
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