//
// Created by Filip Kantedal on 27/09/16.
//

#include <iostream>
#include "DiffuseMaterial.h"

void DiffuseMaterial::BRDF(Ray* ray, vec3 collision_normal) {
    if ((((double) rand()/RAND_MAX) > 0.2)  && !is_emitting_light()) {
        double azimuth = 2 * 3.14 * ((double)rand()/RAND_MAX); //phi
        double inclination = acos(sqrt((double)rand()/RAND_MAX)); //theta
        vec3 reflected_direction = normalize(vec3(sin(inclination)*cos(azimuth), sin(inclination)*sin(azimuth), cos(inclination)));
        vec3 tangent_1 = normalize(cross(collision_normal,vec3(1,0,0)));
        vec3 tangent_2 = normalize(cross(collision_normal, tangent_1));
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