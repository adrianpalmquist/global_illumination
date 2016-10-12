//
// Created by Filip Kantedal on 27/09/16.
//

#include <iostream>
#include "SpecularMaterial.h"

void SpecularMaterial::BRDF(Ray *ray, vec3 collision_normal) {
    vec3 reflected_direction = ReflectRay(ray, collision_normal);
    //ray->set_reflected_ray(nullptr);
    TransmitRay(ray, collision_normal, reflected_direction);

    //ray->set_transmitted_ray(nullptr);
    //ray->get_transmitted_ray()->set_radiance_distribution(1.0);
}

vec3 SpecularMaterial::ReflectRay(Ray* ray, vec3 collision_normal) {
    vec3 reflected_direction = normalize(ray->get_direction() - 2 * dot(ray->get_direction(), collision_normal) * collision_normal);
    Ray* reflected_ray = new Ray(ray, ray->get_end_point(), reflected_direction, 1 + ray->get_ray_iterations());

    reflected_ray->set_radiance_distribution(1.0);
    ray->set_reflected_ray(reflected_ray);
    ray->set_transmitted_ray(nullptr);

    return reflected_direction;
}

void SpecularMaterial::TransmitRay(Ray *ray, vec3 collision_normal, vec3 reflected_direction) {
    float n1, n2;
    float radiance_distribution = 1.0;
    bool incoming = false;
    if (!(collision_normal.x == 0 && collision_normal.y == 0 && collision_normal.z == 0)) {
        if ((glm::dot(collision_normal, ray->get_direction())) < 0.0) {
            incoming = true;
            n1 = 1.000293;
            n2 = 1.52;
        }
        else {
            n1 = 1.52;
            n2 = 1.000293;
        }

        vec3 transmitted_direction = normalize((n1 / n2) * ray->get_direction() + collision_normal * (float) ((
                -(n1 / n2) * dot(collision_normal, ray->get_direction()) -
                sqrt(1.0 - pow((n1 / n2), 2) * (1.0 - (pow(dot(collision_normal, ray->get_direction()), 2)))))));

        float angle = acos(dot(-ray->get_direction(), collision_normal));
        float r_s = (float) (pow((n1 * cos(angle) - n2 * sqrt(1.0 - pow((n1 / n2) * sin(angle), 2))) /
                                 (n1 * cos(angle) + n2 * sqrt(1.0 - pow((n1 / n2) * sin(angle), 2))), 2));
        float r_p = (float) (pow((n1 * sqrt(1.0 - pow((n1 / n2) * sin(angle), 2)) - n2 * cos(angle)) /
                                 (n1 * sqrt(1.0 - pow((n1 / n2) * sin(angle), 2)) + n2 * cos(angle)), 2));
        radiance_distribution = (r_s + r_p) / 2;

        if (!incoming && angle > 0.73) {
            radiance_distribution = 1.0;
        }

        Ray *transmitted_ray = new Ray(ray, ray->get_end_point(), transmitted_direction, 1 + ray->get_ray_iterations());
        transmitted_ray->set_radiance_distribution(1 - radiance_distribution);
        ray->set_transmitted_ray(transmitted_ray);
        ray->get_reflected_ray()->set_radiance_distribution(radiance_distribution);
    }
}
