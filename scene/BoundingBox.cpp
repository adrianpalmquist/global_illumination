//
// Created by Filip Kantedal on 2016-10-11.
//

#include "BoundingBox.h"
#include "../glm/glm.hpp"
#include "../Renderer.h"

float sum_vec(vec3 vec) {
    return vec.x + vec.y + vec.z;
}

BoundingBox::BoundingBox() {
    min = vec3(0,0,0);
    max = vec3(0,0,0);

    // See sketch in assets folder for construction details
    v1 = vec3(min.x, min.y, min.z);
    v2 = vec3(max.x, min.y, min.z);
    v3 = vec3(max.x, max.y, min.z);
    v4 = vec3(min.x, max.y, min.z);

    v5 = vec3(min.x, min.y, max.z);
    v6 = vec3(max.x, min.y, max.z);
    v7 = vec3(max.x, max.y, max.z);
    v7 = vec3(min.x, max.y, max.z);
}


BoundingBox::BoundingBox(vec3 _min, vec3 _max) {
    min = _min;
    max = _max;

    // See sketch in assets folder for construction details
    v1 = vec3(min.x, min.y, min.z);
    v2 = vec3(max.x, min.y, min.z);
    v3 = vec3(max.x, max.y, min.z);
    v4 = vec3(min.x, max.y, min.z);

    v5 = vec3(min.x, min.y, max.z);
    v6 = vec3(max.x, min.y, max.z);
    v7 = vec3(max.x, max.y, max.z);
    v7 = vec3(min.x, max.y, max.z);
}

bool BoundingBox::Contains(vec3 p) {
    vec3 closest_point = glm::min(glm::max(p, min), max);
    float distance_squared = sum_vec((closest_point - p) * (closest_point - p));
    float photon_radius = Renderer::PHOTON_RADIUS;
    return distance_squared < photon_radius * photon_radius;
    //return false;
}

const vec3 BoundingBox::get_min() {
    return min;
}

const vec3 BoundingBox::get_max() {
    return max;
}

bool BoundingBox::ConainsPoint(vec3 p) {
    if (!(p.x >= get_min().x && p.x <= get_max().x))
        return false;

    if (!(p.y >= get_min().y && p.y <= get_max().y))
        return false;

    if (!(p.z >= get_min().z && p.z <= get_max().z))
        return false;

    return true;
}

bool BoundingBox::RayIntersection(Ray ray) {
    float tmin = (min.x - ray.get_start_point().x) / ray.get_direction().x;
    float tmax = (max.x - ray.get_start_point().x) / ray.get_direction().x;

    if (tmin > tmax) {
        float temp = tmin;
        tmin = tmax;
        tmax = temp;
    }

    float tymin = (min.y - ray.get_start_point().y) / ray.get_direction().y;
    float tymax = (max.y - ray.get_start_point().y) / ray.get_direction().y;

    if (tymin > tymax) {
        float temp = tymin;
        tymin = tymax;
        tymax = temp;
    }

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (min.z - ray.get_start_point().z) / ray.get_direction().z;
    float tzmax = (max.z - ray.get_start_point().z) / ray.get_direction().z;

    if (tzmin > tzmax) {
        float temp = tzmin;
        tzmin = tzmax;
        tzmax = temp;
    }

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    return true;
}

