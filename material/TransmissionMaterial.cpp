//
// Created by Filip K on 18/10/16.
//

#include <iostream>
#include "TransmissionMaterial.h"

ColorRGB TransmissionMaterial::BRDF(vec3 incoming_direction, vec3 outgoing_direction, vec3 collision_normal) {
    return get_color();
}

void TransmissionMaterial::PDF(vec3 ray_direction, vec3 collision_normal, vec3 &reflected_dir, vec3 &transmitted_dir, float &radiance_dist) {
    bool incoming_ray = dot(collision_normal, ray_direction) < 0.0f;

    // Should not reflect ray if inside object
    if (incoming_ray)
        ReflectRay(ray_direction, collision_normal, reflected_dir);
    TransmitRay(ray_direction, collision_normal, reflected_dir, radiance_dist, incoming_ray, transmitted_dir);
}

void TransmissionMaterial::ReflectRay(vec3 direction, vec3 collision_normal, vec3 &reflected_direction) {
    reflected_direction = normalize(direction - 2 * dot(direction, collision_normal) * collision_normal);
}

void TransmissionMaterial::TransmitRay(vec3 direction, vec3 collision_normal, vec3 reflected_direction, float &radiance_dist, bool incoming_ray, vec3 &transmitted_direction) {
    float n1, n2;
    if (!(collision_normal.x == 0 && collision_normal.y == 0 && collision_normal.z == 0)) {
        vec3 real_normal = incoming_ray ? collision_normal : -collision_normal;

        if (incoming_ray) {
            n1 = 1.0f;
            n2 = 1.5f;
        }
        else {
            //std::cout << "outgoing" << std::endl;
            n1 = 1.5f;
            n2 = 1.0f;
        }


        transmitted_direction = normalize((n1 / n2) * direction + real_normal * (
                -(n1 / n2) * dot(real_normal, direction) -
                sqrtf(1.0f - powf((n1 / n2), 2.0f) * (1.0f - (powf(dot(real_normal, direction), 2))))));

        float angle = acosf(dot(-direction, real_normal));
        float r_s = powf((n1 * cosf(angle) - n2 * sqrtf(1.0f - powf((n1 / n2) * sinf(angle), 2.0f))) /
                                 (n1 * cosf(angle) + n2 * sqrtf(1.0f - powf((n1 / n2) * sinf(angle), 2.0f))), 2.0f);
        float r_p = powf((n1 * sqrtf(1.0f - powf((n1 / n2) * sinf(angle), 2)) - n2 * cosf(angle)) /
                                 (n1 * sqrtf(1.0f - pow((n1 / n2) * sinf(angle), 2.0f)) + n2 * cosf(angle)), 2.0f);

        radiance_dist = (r_s + r_p) / 2;
        //radiance_dist = 0.0f;

        if (!incoming_ray && angle > 0.73f) {
            radiance_dist = 1.0f;
        }
    }
}
