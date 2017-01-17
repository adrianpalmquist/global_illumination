//
// Created by Filip Kantedal on 23/09/16.
//

#include "Ray.h"

Ray::Ray(vec3 _start_point, vec3 _direction, int _ray_iteration)
: start_point(_start_point), direction(_direction), ray_iteration(_ray_iteration) {
    ray_clr = ColorRGB(0,0,0);
}

Ray::Ray() {
    ray_clr = ColorRGB(0,0,0);
    start_point = vec3(0);
    end_point = vec3(0);
    ray_iteration = 0;
}

Ray::~Ray() {}

ColorRGB Ray::get_ray_color() {
    return ray_clr;
}

vec3 Ray::get_direction() {
    return direction;
}

vec3 Ray::get_start_point() {
    return start_point;
}

void Ray::set_ray_color(ColorRGB _ray_clr) {
    ray_clr = _ray_clr;
}

void Ray::set_end_point(vec3 _end_point) {
    end_point = _end_point;
}

int Ray::get_ray_iterations() {
    return ray_iteration;
}

vec3 Ray::get_end_point() {
    return end_point;
}

void Ray::set_radiance_distribution(float _radiance_distribution) {
    radiance_distribution = _radiance_distribution;
}

float Ray::get_radiance_distribution() {
    return radiance_distribution;
}

std::vector<Ray *> Ray::get_shadow_rays() {
    return shadow_rays;
}

void Ray::add_shadow_ray(Ray *shadow_ray) {
    shadow_rays.push_back(shadow_ray);
}


void Ray::add_ray_color(ColorRGB _ray_clr) {
    ray_clr += _ray_clr;
}