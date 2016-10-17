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

// Returns the mean radiance from surrounding photons
ColorDbl RayTracer::MeanFromPhotonMap(vec3 position, vec3 object_normal) {
    ColorDbl radiance(0,0,0);
    std::vector<Photon*> photons = photon_map->GetIntersections(position);

    int photon_count = 0;
    for (int i = 0; i < photons.size(); i++) {
        if (dot(-photons.at(i)->get_direction(), object_normal) > 0) {
            radiance += photons.at(i)->get_radiance();
            photon_count++;
        }
    }

    double photon_radius = Renderer::PHOTON_RADIUS;
    radiance = radiance * (1.0 / M_PI) * photon_radius;

    return radiance;
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

        float u, v;
        while (ray_count < Renderer::NUM_SHADOW_RAYS) {

            // Randomize two points on the emission triangle using Baycentric coordinate
            u = ((float) rand() / (RAND_MAX)), v = ((float) rand() / (RAND_MAX));

            // Check so that the coordinate sum is less than 1
            if (u + v < 1) {
                vec3 ray_endpoint = triangle->BarycentricToCartesian(u, v);
                vec3 ray_direction = ray_endpoint - collision_point;
                vec3 ray_collision_pos;

                if (scene->RayIntersection(collision_point, ray_direction, ray_collision_pos)) {
                    float distance_to_light = length(ray_direction);
                    float distance_to_collision = length(ray_collision_pos - collision_point);

                    // Check if ray has collided with object before the emission triangle
                    if (distance_to_light <= distance_to_collision + 0.01) {
                        radiance_factor += emitting_material->get_flux() * 1.0f / pow(distance_to_light, 1.8f);
                    }
                }

                ray_count++;
            }
        }

        // Add to radiance factor
        radiance_factor /= (float) Renderer::NUM_SHADOW_RAYS;
        radiance_from_light += emitting_material->get_light_color() * radiance_factor;
    }
    return radiance_from_light;
}

ColorDbl RayTracer::TraceRay(Ray *ray) {
    ColorDbl total_radiance(0,0,0);
    vec3 collision_normal, collision_pos;
    BaseMaterial* collision_material = nullptr;

    if (scene->RayIntersection(*ray->get_start_point(), ray->get_direction(), collision_pos, collision_normal, collision_material)) {
        // Set ray end point
        ray->set_end_point(new vec3(collision_pos.x, collision_pos.y, collision_pos.z));

        // Return if ray exceeds max iterations
        if (ray->get_ray_iterations() > 5 || collision_material == nullptr) return ColorDbl(0,0,0);

        if (collision_material->is_emitting_light()) {
            return collision_material->get_light_color() * collision_material->get_flux();
        }

        // Create child rays
        vec3 reflected_dir, transmitted_dir = vec3(0.0);
        float radiance_distribution = 0.0;
        collision_material->PDF(ray->get_direction(), collision_normal, reflected_dir, transmitted_dir, radiance_distribution);

        if (length(reflected_dir) != 0) {
            Ray* reflected_ray = new Ray(ray, ray->get_end_point(), reflected_dir, 1 + ray->get_ray_iterations());
            reflected_ray->set_radiance_distribution(radiance_distribution);
            ray->set_reflected_ray(reflected_ray);
            ray->set_transmitted_ray(nullptr);
        }

        if (length(transmitted_dir) != 0) {
            Ray *transmitted_ray = new Ray(ray, ray->get_end_point(), transmitted_dir, 1 + ray->get_ray_iterations());
            transmitted_ray->set_radiance_distribution(1 - radiance_distribution);
            ray->set_transmitted_ray(transmitted_ray);
            ray->get_reflected_ray()->set_radiance_distribution(radiance_distribution);
        }

        ColorDbl light_radiance, reflectance_radiance, transmission_radiance;
        if (collision_material->get_material_type() == BaseMaterial::DIFFUSE) {
            light_radiance = TraceShadowRays(ray, collision_pos) * collision_material->BRDF(ray->get_direction(), vec3(0), collision_normal);

            if (ray->get_reflected_ray() != nullptr) {
                ColorDbl reflected_radiance = TraceRay(ray->get_reflected_ray());
                reflectance_radiance = reflected_radiance * ray->get_reflected_ray()->get_radiance_distribution() * collision_material->BRDF(ray->get_direction(), vec3(0), collision_normal);
            }

            return light_radiance + reflectance_radiance;
        }
        else if (collision_material->get_material_type() == BaseMaterial::SPECULAR) {
            if (ray->get_reflected_ray() != nullptr) {
                ColorDbl reflected_radiance = TraceRay(ray->get_reflected_ray());
                reflectance_radiance = reflected_radiance * ray->get_reflected_ray()->get_radiance_distribution() * collision_material->BRDF(ray->get_direction(), vec3(0), collision_normal);
            }

            if (ray->get_transmitted_ray() != nullptr) {
                ColorDbl transmitted_radiance = TraceRay(ray->get_transmitted_ray());
                transmission_radiance = transmitted_radiance * ray->get_transmitted_ray()->get_radiance_distribution();
            }

            return reflectance_radiance + transmission_radiance;
        }


    }

    return ColorDbl(0,0,0);
}

void RayTracer::set_photon_map(PhotonOctree* _photon_map) {
    photon_map = _photon_map;
}




