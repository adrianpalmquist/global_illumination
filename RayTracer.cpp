//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include "RayTracer.h"
#include "Renderer.h"


RayTracer::RayTracer() {}


RayTracer::RayTracer(Scene *_scene): scene(_scene) {}

void RayTracer::StartRayTracing(Ray base_ray) {
    TraceRay(base_ray);
}

// Returns the mean radiance from surrounding photons
ColorRGB RayTracer::MeanFromPhotonMap(vec3 position, vec3 object_normal) {
    ColorRGB radiance(0,0,0);
    std::vector<Photon*> photons = photon_map->GetIntersections(position);

    int photon_count = 0;
    for (int i = 0; i < photons.size(); i++) {
        if (dot(-photons.at(i)->get_direction(), object_normal) > 0) {
            radiance += photons.at(i)->get_radiance();
            photon_count++;
        }
    }

    float photon_radius = Renderer::PHOTON_RADIUS;
    radiance = radiance * (1.0f / ((float) M_PI)) * photon_radius;

    return radiance;
}


// Returns radiance contribution from light sources
ColorRGB RayTracer::TraceShadowRays(Ray ray, vec3 collision_point) {
    ColorRGB radiance_from_light = ColorRGB(0,0,0);

    std::vector<Triangle*> light_emitting_triangles = scene->get_light_emitting_triangles();
    for (std::vector<Triangle*>::iterator it = light_emitting_triangles.begin(); it != light_emitting_triangles.end(); ++it) {
        Triangle *triangle = *it;
        BaseMaterial *emitting_material = triangle->get_material();

            // Generate random rays from collision point to surface
            int ray_count = 0;
            float radiance_factor = 0.0f;

            float u, v;
            while (ray_count < Renderer::NUM_SHADOW_RAYS) {

                // Randomize two points on the emission triangle using Baycentric coordinate
                u = ((float) rand() / (RAND_MAX)), v = ((float) rand() / (RAND_MAX));

                // Check so that the coordinate sum is less than 1
                if (u + v < 1) {
                    vec3 ray_endpoint = triangle->BarycentricToCartesian(u, v);
                    vec3 ray_direction = ray_endpoint - collision_point;
                    vec3 ray_collision_pos, ray_collision_norm;
                    BaseMaterial *ray_collision_material;
                    float angle_factor;

                    if ((angle_factor = -dot(normalize(ray_direction), triangle->get_normal())) > 0.0f) {
                        // Check so that ray is not coming in from behind
                        Ray new_ray(collision_point, ray_direction, 0);
                    if (scene->RayIntersection(new_ray, ray_collision_pos, ray_collision_norm,
                                               ray_collision_material)) {
                        float distance_to_light = length(ray_direction);
                        float distance_to_collision = length(ray_collision_pos - collision_point);

                        // Check if ray has collided with object before the emission triangle
                        if (distance_to_light <= distance_to_collision + 0.01) {
                            radiance_factor += emitting_material->get_flux() * sqrt(angle_factor) / pow(distance_to_light, 2.0f);
                        }

                    }
                }
                    ray_count++;
                }
            }

            // Add to radiance factor
            radiance_factor /= (float) Renderer::NUM_SHADOW_RAYS;
            radiance_from_light += emitting_material->get_color() * radiance_factor;
    }
    return radiance_from_light;
}

ColorRGB RayTracer::TraceRay(Ray ray) {
    vec3 collision_normal, collision_pos;
    BaseMaterial* collision_material = nullptr;

    if (scene->RayIntersection(ray, collision_pos, collision_normal, collision_material)) {
        // Set ray end point
        ray.set_end_point(vec3(collision_pos.x, collision_pos.y, collision_pos.z));

        // Return if ray exceeds max iterations
        if (ray.get_ray_iterations() > 5 || collision_material == nullptr) return ColorRGB(0,0,0);

        // Create child rays
        vec3 reflected_dir, transmitted_dir = vec3(0.0);
        Ray reflected_ray, transmitted_ray;
        bool ray_reflected = false;

        float radiance_distribution = 1.0f;
        collision_material->PDF(ray.get_direction(), collision_normal, reflected_dir, transmitted_dir, radiance_distribution);

        if (!(reflected_dir.x == 0 && reflected_dir.y == 0 && reflected_dir.z == 0)) {
            reflected_ray = Ray(ray.get_end_point() + reflected_dir * 0.05f, reflected_dir, 1 + ray.get_ray_iterations());
            ray_reflected = true;
        }

        if (!(transmitted_dir.x == 0 && transmitted_dir.y == 0 && transmitted_dir.z == 0)) {
            transmitted_ray = Ray(ray.get_end_point() + transmitted_dir * 0.05f, transmitted_dir, 1 + ray.get_ray_iterations());
        }

        ColorRGB light_radiance, reflectance_radiance, transmission_radiance;
        int material_type = collision_material->get_material_type();

        // Handle diffuse case
        if (material_type == BaseMaterial::DIFFUSE) {
            ColorRGB incoming_light_radiance = TraceShadowRays(ray, collision_pos);
            light_radiance = incoming_light_radiance * collision_material->BRDF(ray.get_direction(), vec3(0), collision_normal);

            if (ray_reflected) {
                reflectance_radiance = TraceRay(reflected_ray) * collision_material->BRDF(ray.get_direction(), vec3(0), collision_normal);
            }

            return light_radiance + reflectance_radiance;
        }

        // Handle specular/perfect reflection case
        if (material_type == BaseMaterial::SPECULAR) {
            return TraceRay(reflected_ray) * collision_material->BRDF(ray.get_direction(), vec3(0), collision_normal);
        }

        // Handle transmitting cae
        if (material_type == BaseMaterial::TRANSMITTING) {
            return TraceRay(transmitted_ray) * (1.0f - radiance_distribution) + TraceRay(reflected_ray) * radiance_distribution;
        }


        // Handle emitting case
        if (material_type == BaseMaterial::EMISSION) {
            return collision_material->BRDF(ray.get_direction(), vec3(0), collision_normal);
        }
    }

    return ColorRGB(0,0,0);
}

void RayTracer::set_photon_map(PhotonOctree* _photon_map) {
    photon_map = _photon_map;
}




