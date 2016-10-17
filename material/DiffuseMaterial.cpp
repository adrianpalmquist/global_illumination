//
// Created by Filip Kantedal on 27/09/16.
//

#include <iostream>
#include "DiffuseMaterial.h"

ColorDbl DiffuseMaterial::BRDF(vec3 incoming_direction, vec3 outgoing_direction, vec3 collision_normal) {
    return get_color() * (1.0 / M_PI *  ((double) dot(normalize(-incoming_direction), normalize(collision_normal))));
}

void DiffuseMaterial::PDF(vec3 ray_direction, vec3 collision_normal, vec3 &reflected_dir, vec3 &transmitted_dir, float &radiance_dist) {
    if ((((float) rand()/RAND_MAX) < 0.95)  && !is_emitting_light()) {
        while (true) {
            vec3 b((((float) rand()) / RAND_MAX) - 0.5f, (((float) rand()) / RAND_MAX) - 0.5f, (((float) rand()) / RAND_MAX) - 0.5f);
            b = normalize(b);

            if (dot(b, collision_normal) > 0) {
                reflected_dir = b;
                break;
            }
        }
        radiance_dist = 1.0f;
    }
}


