//
// Created by Filip Kantedal on 23/09/16.
//

#include "Ray.h"

Ray::Ray(Ray* _parent_ray, vec3 *_start_point, vec3 _direction, int _ray_iteration)
: parent_ray(_parent_ray), start_point(_start_point), direction(_direction), ray_iteration(_ray_iteration) {
    reflected_ray = 0;
    transmitted_ray = 0;
    ray_clr = ColorDbl(0,0,0);
}

Ray::~Ray() {

}

ColorDbl Ray::get_ray_color() {
    return ray_clr;
}

Ray *Ray::get_reflected_ray() {
    return reflected_ray;
}

Ray *Ray::get_transmitted_ray() {
    return transmitted_ray;
}

vec3 Ray::get_direction() {
    return direction;
}

vec3 *Ray::get_start_point() {
    return start_point;
}

void Ray::set_ray_color(ColorDbl _ray_clr) {
    ray_clr = _ray_clr;
}

void Ray::set_end_point(vec3 *_end_point) {
    end_point = _end_point;
}

int Ray::get_ray_iterations() {
    return ray_iteration;
}

void Ray::set_transmitted_ray(Ray *_transmitted_ray) {
    transmitted_ray = _transmitted_ray;
}

void Ray::set_reflected_ray(Ray *_reflected_ray) {
    reflected_ray = _reflected_ray;
}

vec3 *Ray::get_end_point() {
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

Ray *Ray::get_parent_ray() {
    return parent_ray;
}

void Ray::add_ray_color(ColorDbl _ray_clr) {
    ray_clr += _ray_clr;
}

