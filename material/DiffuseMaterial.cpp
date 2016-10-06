//
// Created by Filip Kantedal on 27/09/16.
//

#include "DiffuseMaterial.h"

void DiffuseMaterial::BRDF(Ray* ray, vec3 collision_normal) {
    if (rand()/RAND_MAX > 0.2  && !is_emitting_light()) {
        double azimuth = 2 * 3.14 * (rand()/RAND_MAX);
        double inclination = acos(sqrt(rand()/RAND_MAX));
        vec3 reflected_direction = vec3(cos(azimuth)*cos(inclination), sin(azimuth)*sin(inclination), cos(inclination));
        vec3 tangent_1 = cross(collision_normal,vec3(0,0,1));
        vec3 tangent_2 = cross(collision_normal, tangent_1);
        mat3 rotationMatrix = mat3(tangent_1,tangent_2,collision_normal);
        reflected_direction = transpose(rotationMatrix)*reflected_direction;
        Ray *reflected_ray = new Ray(ray, ray->get_end_point(), reflected_direction, 1 + ray->get_ray_iterations());
        reflected_ray->set_radiance_distribution(1.0);
        ray->set_reflected_ray(reflected_ray);
    }
    else {
        ray->set_reflected_ray(nullptr);
    }
    ray->set_transmitted_ray(nullptr);
}