//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include "RayTracer.h"
#include "Renderer.h"


RayTracer::RayTracer() {}


RayTracer::RayTracer(Scene *_scene): scene(_scene) {}

void RayTracer::StartRayTracing(Ray base_ray) {
    TraceRay(base_ray, true);
}

// Returns the mean radiance from surrounding photons
ColorRGB RayTracer::MeanFromPhotonMap(vec3 position, vec3 object_normal) {
    ColorRGB radiance(0,0,0);
    std::vector<Photon*> photons = photon_map->GetIntersections(position);

    for (int i = 0; i < photons.size(); i++) {
        if (dot(-photons.at(i)->get_direction(), object_normal) > 0) {
            radiance += photons.at(i)->get_radiance();
        }
    }

    float photon_radius = Renderer::PHOTON_RADIUS;
    radiance = radiance * (1.0f / ((float) M_PI) * photon_radius * photon_radius) / 1000.0f;

    return radiance;
}


// Returns radiance contribution from light sources
ColorRGB RayTracer::TraceShadowRays(Ray ray, vec3 collision_point, vec3 surface_normal) {
    ColorRGB radiance_from_light = ColorRGB(0,0,0);
    float radiance_factor = 0.0;

    std::vector<Object3d> light_emitting_objects = scene->get_light_emitting_objects();
    for (std::vector<Object3d>::iterator it = light_emitting_objects.begin(); it != light_emitting_objects.end(); ++it) {
        Object3d object = *it;
        BaseMaterial* emitting_material = object.get_material();
        radiance_from_light = emitting_material->get_color();
        int num_triangles = object.get_triangles().size();

        float accumulated_area = 0.0;
        // Shadow
        for (int i = 0; i < Renderer::NUM_SHADOW_RAYS; i++) {
            // Randomize which triangle to be tested
            int triangle_index = (int) floor(((float) (rand() / RAND_MAX)) * num_triangles);
            Triangle* emitting_triangle = object.get_triangles().at(triangle_index);

            float u = ((float) rand() / (RAND_MAX));
            float v = ((float) rand() / (RAND_MAX)) * (1.0f - u);

            vec3 ray_endpoint = emitting_triangle->BarycentricToCartesian(u, v);
            vec3 ray_direction = ray_endpoint - collision_point;
            vec3 ray_collision_pos, ray_collision_norm;
            BaseMaterial *ray_collision_material;
            float angle_factor = -dot(normalize(ray_direction), emitting_triangle->get_normal());

            if (angle_factor > 0.0f) {
                Ray new_ray(collision_point + surface_normal * 0.01f, ray_direction, 0);
                if (scene->RayIntersection(new_ray, ray_collision_pos, ray_collision_norm, ray_collision_material)) {
                    float distance_to_light = length(ray_direction);
                    float distance_to_collision = length(ray_collision_pos - collision_point);

                    // Check if ray has collided with object before the emission triangle
                    if (distance_to_light <= distance_to_collision + 0.01) {
                        float alpha = dot(surface_normal, normalize(ray_direction));
                        float beta = dot(emitting_triangle->get_normal(), -normalize(ray_direction));
                        radiance_factor += alpha * beta * emitting_material->get_flux() / pow(distance_to_light, 2.0f);
                    }
                }
            }
        }

        radiance_factor = radiance_factor / ((float) Renderer::NUM_SHADOW_RAYS);
    }

    return radiance_from_light * radiance_factor;
}

ColorRGB RayTracer::TraceRay(Ray ray, bool perform_full_calc) {
    vec3 collision_normal, collision_pos;
    BaseMaterial* collision_material = nullptr;

    if (scene->RayIntersection(ray, collision_pos, collision_normal, collision_material)) {
        // Set ray end point
        ray.set_end_point(vec3(collision_pos.x, collision_pos.y, collision_pos.z));

        // Return if ray exceeds max iterations
        if (ray.get_ray_iterations() > 5 || collision_material == nullptr) return ColorRGB(0,0,0);

        //return MeanFromPhotonMap(collision_pos, collision_normal);

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

        // Handle specular/perfect reflection case
        if (material_type == BaseMaterial::SPECULAR) {
            if (perform_full_calc) {
                return TraceRay(reflected_ray, true) * collision_material->BRDF(ray.get_direction(), vec3(0), collision_normal);
            }
            else {
                return TraceRay(reflected_ray, false) * collision_material->BRDF(ray.get_direction(), vec3(0), collision_normal);
            }
        }

        // Handle transmitting cae
        else if (material_type == BaseMaterial::TRANSMITTING) {
            if (!ray_reflected) return TraceRay(transmitted_ray,true);
            return TraceRay(transmitted_ray, true) * (1.0f - radiance_distribution) + TraceRay(reflected_ray, true) * radiance_distribution;
        }

        // Handle emitting case
        else if (material_type == BaseMaterial::EMISSION && perform_full_calc) {
            return collision_material->BRDF(ray.get_direction(), vec3(0), collision_normal);
        }

        // Handle diffuse case
        else if (material_type == BaseMaterial::DIFFUSE || material_type == BaseMaterial::OREN_NAYAR) {
            if (!perform_full_calc) {
                return MeanFromPhotonMap(collision_pos, collision_normal);
            }
            else {
                ColorRGB incoming_light_radiance = TraceShadowRays(ray, collision_pos, collision_normal);
                light_radiance = incoming_light_radiance * collision_material->BRDF(ray.get_direction(), reflected_dir, collision_normal);


                if (ray_reflected) {
                    reflectance_radiance = TraceRay(reflected_ray, false) * collision_material->BRDF(ray.get_direction(), reflected_dir, collision_normal);
                }

                return light_radiance + reflectance_radiance;
            }

        }
    }

    return ColorRGB(0,0,0);
}

void RayTracer::set_photon_map(PhotonOctree* _photon_map) {
    photon_map = _photon_map;
}




