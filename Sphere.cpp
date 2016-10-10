//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include <vector>
#include "Sphere.h"

Sphere::Sphere(vec3 _position, float _radius, BaseMaterial* _material): position(_position), radius(_radius), material(_material) {}

float sum(vec3 vec) {
    //return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
    return vec.x + vec.y + vec.z;
}

bool Sphere::RayIntersection(vec3 origin, vec3 direction, vec3 &intersection_point, vec3 &normal) {
    const vec3 displacement = origin - position;
    const float a = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;
    const float b = 2.0f * dot(direction, displacement);
    const float c = sum(displacement * displacement) - radius*radius;

    const float radicand = b * b - 4.0f * a * c;
    std::vector<vec3> intersections;
    if (radicand >= 0.0f) {
        const float root = sqrt(radicand);
        const float denom = 2.0f * a;
        const float u[2] = {
                (-b + root) / denom,
                (-b - root) / denom
        };

        for (int i = 0; i < 2; i++) {
            if (u[i] > 0.0001) {
                const vec3 originToSurface = u[i] * direction;
                intersections.push_back(origin + originToSurface);
            }
        }

        if (intersections.size() != 0) {
            intersection_point = intersections.at(0);
            normal = (intersection_point - position) / radius;
            return true;
        }

        return false;

    }

    return false;
}

vec3 Sphere::get_position() {
    return position;
}

float Sphere::get_radius() {
    return radius;
}

BaseMaterial* Sphere::get_material() {
    return material;
}