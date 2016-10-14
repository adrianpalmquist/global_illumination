//
// Created by Filip Kantedal on 27/09/16.
//

#include <iostream>
#include "SpecularMaterial.h"

void SpecularMaterial::BRDF(vec3 ray_direction, vec3 collision_normal, vec3 &reflected_dir, vec3 &transmitted_dir, float &radiance_dist) {
    ReflectRay(ray_direction, collision_normal, reflected_dir);
    TransmitRay(ray_direction, collision_normal, reflected_dir, radiance_dist, transmitted_dir);
}

void SpecularMaterial::ReflectRay(vec3 direction, vec3 collision_normal, vec3 &reflected_direction) {
    reflected_direction = normalize(direction - 2 * dot(direction, collision_normal) * collision_normal);
}

void SpecularMaterial::TransmitRay(vec3 direction, vec3 collision_normal, vec3 reflected_direction, float &radiance_dist, vec3 &transmitted_direction) {
    float n1, n2;
    bool incoming = false;
    if (!(collision_normal.x == 0 && collision_normal.y == 0 && collision_normal.z == 0)) {
        if ((glm::dot(collision_normal, direction)) < 0.0) {
            incoming = true;
            n1 = 1.000293;
            n2 = 1.52;
        } else {
            n1 = 1.52;
            n2 = 1.000293;
        }

        transmitted_direction = normalize((n1 / n2) * direction + collision_normal * (float) ((
                -(n1 / n2) * dot(collision_normal, direction) -
                sqrt(1.0 - pow((n1 / n2), 2) * (1.0 - (pow(dot(collision_normal, direction), 2)))))));

        float angle = acos(dot(-direction, collision_normal));
        float r_s = (float) (pow((n1 * cos(angle) - n2 * sqrt(1.0 - pow((n1 / n2) * sin(angle), 2))) /
                                 (n1 * cos(angle) + n2 * sqrt(1.0 - pow((n1 / n2) * sin(angle), 2))), 2));
        float r_p = (float) (pow((n1 * sqrt(1.0 - pow((n1 / n2) * sin(angle), 2)) - n2 * cos(angle)) /
                                 (n1 * sqrt(1.0 - pow((n1 / n2) * sin(angle), 2)) + n2 * cos(angle)), 2));
        radiance_dist= (r_s + r_p) / 2;

        if (!incoming && angle > 0.73) {
            radiance_dist = 1.0;
        }
    }
}
