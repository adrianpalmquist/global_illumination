//
// Created by Filip Kantedal on 27/09/16.
//

#include <iostream>
#include "SpecularMaterial.h"

ColorRGB SpecularMaterial::BRDF(vec3 incoming_direction, vec3 outgoing_direction, vec3 collision_normal) {
    return get_color();
}

void SpecularMaterial::PDF(vec3 ray_direction, vec3 collision_normal, vec3 &reflected_dir, vec3 &transmitted_dir, float &radiance_dist) {
    ReflectRay(ray_direction, collision_normal, reflected_dir);
}

void SpecularMaterial::ReflectRay(vec3 direction, vec3 collision_normal, vec3 &reflected_direction) {
    reflected_direction = normalize(direction - 2 * dot(direction, collision_normal) * collision_normal);
}