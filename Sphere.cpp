//
// Created by Filip Kantedal on 23/09/16.
//

#include "Sphere.h"

Sphere::Sphere(vec3 _position, float _radius, BaseMaterial* _material): position(_position), radius(_radius), material(_material) {}

float sum(vec3 vec) {
    //return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
    return vec.x + vec.y + vec.z;
}

bool Sphere::RayIntersection(vec3 origin, vec3 direction, vec3 &intersection_point, vec3 &normal) {
    float a = sum(direction * direction);
    float b = sum(direction * (2.0f * ( origin - position)));
    float c = sum(position * position) + sum(origin * origin) -2.0f*sum(origin * position) - radius * radius;
    float D = b*b + (-4.0f)*a*c;

    // If ray can not intersect then stop
    if (D < 0)
        return false;
    D=sqrtf(D);

    // Ray can intersect the sphere, solve the closer hitpoint
    float t = (-0.5f)*(b+D)/a;
    if (t > 0.0f)
    {
        intersection_point = origin + t*direction;
        normal = (intersection_point - position) / radius;
    }
    else
        return false;
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