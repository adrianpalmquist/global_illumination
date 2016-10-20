//
// Created by Filip Kantedal on 27/09/16.
//

#include <iostream>
#include "DiffuseMaterial.h"

ColorRGB DiffuseMaterial::BRDF(vec3 incoming_direction, vec3 outgoing_direction, vec3 collision_normal) {
    return get_color() * ( 1.0 / (M_PI * ((float) dot(normalize(-incoming_direction), normalize(collision_normal))) ));
}

void DiffuseMaterial::PDF(vec3 ray_direction, vec3 collision_normal, vec3 &reflected_dir, vec3 &transmitted_dir, float &radiance_dist) {
    if ((((float) rand()/RAND_MAX) < 0.8)  && !is_emitting_light()) {
        float r1 = 2 * 3.14159f * ((float) rand() / RAND_MAX);// pick random number on unit circle (radius = 1, circumference = 2*Pi) for azimuth
        float r2 = ((float) rand() / RAND_MAX);  // pick random number for elevation
        float r2s = sqrtf(r2);

        vec3 w = collision_normal;
        vec3 u = normalize(cross((fabs(w.x) > .1 ? vec3(0, 1, 0) : vec3(1, 0, 0)), w));
        vec3 v = cross(w,u);

        reflected_dir = normalize(u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrtf(1 - r2));
        radiance_dist = 1.0f;
    }
}


