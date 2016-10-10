//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include "RayTracer.h"
#include "Renderer.h"


RayTracer::RayTracer() {}


RayTracer::RayTracer(Scene *_scene): scene(_scene) {}

void RayTracer::StartRayTracing(Ray *base_ray) {
    ray_tree = base_ray;
    TraceRay(base_ray);
}

void RayTracer::ReflectRay(Ray* ray, vec3 collision_normal) {
    if (!(collision_normal.x == 0 && collision_normal.y == 0 && collision_normal.z == 0)) {
        vec3 reflected_direction = normalize(ray->get_direction() - 2 * dot(ray->get_direction(), collision_normal) * collision_normal);
        Ray *reflected_ray = new Ray(ray, ray->get_end_point(), reflected_direction, 1 + ray->get_ray_iterations());
        ray->set_reflected_ray(reflected_ray);
    }
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

// Returns radiance contribution from light sources
ColorDbl RayTracer::TraceShadowRays(Ray *ray, vec3 collision_point) {
    ColorDbl radiance_from_light = ColorDbl(0,0,0);

    std::vector<Triangle*> light_emitting_triangles = scene->get_light_emitting_triangles();
    for (std::vector<Triangle*>::iterator it = light_emitting_triangles.begin(); it != light_emitting_triangles.end(); ++it) {
        Triangle *triangle = *it;
        BaseMaterial *emitting_material = triangle->get_material();

        // Generate random rays from collision point to surface
        int ray_count = 0;
        float radiance_factor = 0.0f;

        for (int ray_count = 0; ray_count < Renderer::NUM_SHADOW_RAYS; ray_count++) {
            vec3 ray_endpoint = triangle->RandomizePointOnTriangle();
            vec3 ray_direction = ray_endpoint - collision_point;
            vec3 ray_collision_pos;

            if (scene->RayIntersection(collision_point, ray_direction, ray_collision_pos)) {
                float distance_to_light = length(ray_direction);
                float distance_to_collision = length(ray_collision_pos - collision_point);

                // Check if ray has collided with object before the emission triangle
                if (distance_to_light <= distance_to_collision + 0.01) {
                    radiance_factor += emitting_material->get_flux() * 1.0f / pow(distance_to_light, 2.0f);
                }
            }
        }

        // Add to radiance factor
        radiance_factor /= (float) Renderer::NUM_SHADOW_RAYS;
        radiance_from_light += emitting_material->get_light_color() * radiance_factor;
    }
    return radiance_from_light;
}

void RayTracer::TraceRay(Ray *ray) {
    vec3 collision_normal, collision_pos;
    BaseMaterial collision_material;
    bool reflect; // Temporary, should consist of collision material instead

    if (scene->RayIntersection(*ray->get_start_point(), ray->get_direction(), collision_pos, collision_normal, collision_material, reflect)) {
        // Set ray end point
        ray->set_end_point(new vec3(collision_pos.x, collision_pos.y, collision_pos.z));

        // Return if ray exceeds max iterations
        if (ray->get_ray_iterations() > 5 && !reflect) return;

        // Create child rays
        if (reflect) {
            ReflectRay(ray, collision_normal);
            TransmitRay(ray, collision_normal, true, ray->get_reflected_ray()->get_direction());
        }

        if (!collision_material.is_emitting_light()) {
            ColorDbl light_radiance = TraceShadowRays(ray, collision_pos);
            ray->set_ray_color((collision_material.get_color() * light_radiance));
        }
        else
            ray->set_ray_color(collision_material.get_light_color() * collision_material.get_flux());

        // Trace child rays
        if (ray->get_reflected_ray() != nullptr) TraceRay(ray->get_reflected_ray());
        if (ray->get_transmitted_ray() != nullptr) TraceRay(ray->get_transmitted_ray());
    }
}

