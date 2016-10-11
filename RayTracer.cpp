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

float RayTracer::CalculateIntensityFactor(float distance) {

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
    BaseMaterial* collision_material = nullptr;

    if (scene->RayIntersection(*ray->get_start_point(), ray->get_direction(), collision_pos, collision_normal, collision_material)) {
        // Set ray end point
        ray->set_end_point(new vec3(collision_pos.x, collision_pos.y, collision_pos.z));

        // Return if ray exceeds max iterations
        if (ray->get_ray_iterations() > 5 || collision_material == nullptr) return;

        // Child rays get created from material reflection function
        collision_material->BRDF(ray, collision_normal);

        if (!collision_material->is_emitting_light()) {
            ColorDbl light_radiance = TraceShadowRays(ray, collision_pos);
            ray->set_ray_color((collision_material->get_color() * light_radiance));
        }
        else {
            ray->set_ray_color(collision_material->get_light_color() * collision_material->get_flux());
        }

        // Trace child rays
        if (ray->get_reflected_ray() != nullptr) TraceRay(ray->get_reflected_ray());
        if (ray->get_transmitted_ray() != nullptr) TraceRay(ray->get_transmitted_ray());
    }
}

