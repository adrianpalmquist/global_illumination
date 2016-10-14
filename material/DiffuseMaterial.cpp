//
// Created by Filip Kantedal on 27/09/16.
//

#include <iostream>
#include "DiffuseMaterial.h"

void DiffuseMaterial::BRDF(vec3 ray_direction, vec3 collision_normal, vec3 &reflected_dir, vec3 &transmitted_dir, float &radiance_dist) {
    if ((((float) rand()/RAND_MAX) < 0.95)  && !is_emitting_light()) {
//        double azimuth = 2 * 3.14 * ((double) rand() / RAND_MAX); //phi
//        double inclination = acos(sqrt((double) rand() / RAND_MAX)); //theta
//
//        reflected_dir = normalize(vec3(sin(inclination) * cos(azimuth), sin(inclination) * sin(azimuth), cos(inclination)));
//        vec3 tangent_1 = normalize(cross(collision_normal, vec3(1,0,0)));
//        vec3 tangent_2 = normalize(cross(collision_normal, tangent_1));
//
//        mat3 rotationMatrix = mat3(tangent_1, tangent_2, collision_normal);
//        reflected_dir = transpose(rotationMatrix)*reflected_dir;
//        radiance_dist = 1.0f / ((float) M_PI);


        while (true) {
            vec3 b((((float) rand()) / RAND_MAX) - 0.5f, (((float) rand()) / RAND_MAX) - 0.5f, (((float) rand()) / RAND_MAX) - 0.5f);
            b = normalize(b);

            if (dot(b, collision_normal) > 0) {
                reflected_dir = b;
                break;
            }
        }

        float angle_contribution = dot(normalize(-ray_direction), normalize(collision_normal));
        radiance_dist = 1.0f / ((float) M_PI) * angle_contribution;
    }
}