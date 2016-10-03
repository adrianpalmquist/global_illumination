//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include "RayTracer.h"


RayTracer::RayTracer() {}


RayTracer::RayTracer(Scene *_scene): scene(_scene) {}

void RayTracer::StartRayTracing(Ray *base_ray) {
    ray_tree = base_ray;
    TraceRay(base_ray);
}

void RayTracer::ReflectRay(Ray* ray, vec3 collision_normal) {
    bool reflected = true;
    vec3 reflected_direction;
    if (!(collision_normal.x == 0 && collision_normal.y == 0 && collision_normal.z == 0)) {
        reflected_direction = normalize(ray->get_direction() - 2 * dot(ray->get_direction(), collision_normal) * collision_normal);
        // monte carlo
        if (false) { // if diffuse reflector
            if (rand()/RAND_MAX > 0.2) {
                double azimuth = (2 * 3.14 * (rand()/RAND_MAX);
                double inclination = acos(sqrt(rand()/RAND_MAX));
                reflected_direction = vec3(cos(azimuth)*cos(inclination), sin(azimuth)*sin(inclination), cos(inclination));
            }
            else {
                reflected = false;
            }
        }
    }

    if (reflected) {
    Ray *reflected_ray = new Ray(ray, ray->get_end_point(), reflected_direction, 1 + ray->get_ray_iterations());
    ray->set_reflected_ray(reflected_ray);
}

void RayTracer::TransmitRay(Ray* ray, vec3 collision_normal, bool going_in, vec3 reflected_direction) {
    float n1, n2, radiance_distribution;
    if (!(collision_normal.x == 0 && collision_normal.y == 0 && collision_normal.z == 0)) {
        if (going_in) {
            n1 = 1.000293;
            n2 = 1.52;
        }
        else {
            n1 = 1.52;
            n2 = 1.000293;
        }

        vec3 transmitted_direction = normalize((n1 / n2) * reflected_direction + collision_normal * (float) ((
                -(n1 / n2) * dot(collision_normal, reflected_direction) -
                sqrt(1 - pow((n1 / n2), 2) * (1 - (pow(dot(collision_normal, reflected_direction), 2)))))));

        float angle = acos(dot(-ray->get_direction(), collision_normal));

        if (!going_in && angle < 0.73) {
            float r_s = (float) (pow((n1 * cos(angle) - n2 * sqrt(1 - pow((n1 / n2) * sin(angle), 2))) /
                             (n1 * cos(angle) + n2 * sqrt(1 - pow((n1 / n2) * sin(angle), 2))), 2));
            float r_p = (float) (pow((n1 * sqrt(1 - pow((n1 / n2) * sin(angle), 2)) - n2 * cos(angle)) /
                             (n1 * sqrt(1 - pow((n1 / n2) * sin(angle), 2)) + n2 * cos(angle)), 2));
            radiance_distribution = (r_s + r_p) / 2;
        }

        Ray *transmitted_ray = new Ray(ray, ray->get_end_point(), transmitted_direction, 1 + ray->get_ray_iterations());
        ray->set_transmitted_ray(transmitted_ray);
        transmitted_ray->set_radiance_distribution(radiance_distribution);
        ray->get_reflected_ray()->set_radiance_distribution(1 - radiance_distribution);
    }
}

void RayTracer::TraceRay(Ray *ray) {
    vec3 collision_normal, collision_pos;
    bool reflect; // Temporary, should consist of collision material instead

    if (scene->RayIntersection(ray, collision_pos, collision_normal, reflect)) {
        ray->set_end_point(new vec3(collision_pos.x, collision_pos.y, collision_pos.z));

        if(ray->get_ray_iterations() > 5 && !reflect) return;

        // Create child rays
        ReflectRay(ray, collision_normal);
        TransmitRay(ray, collision_normal, true, ray->get_reflected_ray()->get_direction());

        // Trace child rays
        if (ray->get_reflected_ray() != nullptr) TraceRay(ray->get_reflected_ray());
        if (ray->get_transmitted_ray() != nullptr) TraceRay(ray->get_transmitted_ray());
    }
}
