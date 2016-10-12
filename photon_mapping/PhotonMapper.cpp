//
// Created by Filip Kantedal on 2016-10-06.
//

#include <iostream>
#include "../Renderer.h"

PhotonMapper::PhotonMapper() {}

PhotonMapper::PhotonMapper(Scene *_scene) : scene(_scene) {}

vec3 PhotonMapper::CosineDistributeDirection(vec3 normal) {
    float sin_theta = sqrt(((float) rand()) / RAND_MAX);
    float cos_theta = (float) sqrt(1.0 - sin_theta * sin_theta);

    // Random in plane angle;
    float psi = (float) (((float) rand()) / RAND_MAX * 2.0f * M_PI);

    // Generate tangents along plane
    vec3 tangent1, tangent2;
    tangent1 = cross(normal, vec3(0,0,1));
    tangent2 = cross(normal, tangent1);

    vec3 v1, v2, v3;
    v1 = sin_theta * cos(psi) * tangent1;
    v2 = sin_theta * sin(psi) * tangent2;
    v3 = cos_theta * normal;

    return v1 + v2 + v3;
}

void PhotonMapper::EmitPhoton(vec3 emission_pos, vec3 emission_direction, ColorDbl emission_radiance) {
    vec3 collision_pos, collision_normal;
    BaseMaterial* collision_material;
    if (scene->RayIntersection(emission_pos, emission_direction, collision_pos, collision_normal, collision_material)) {
        // Calculate irradiance from emitted radiance and dropoff
        float distance_to_emission = length(emission_pos - collision_pos);
        ColorDbl irradiance = (collision_material->get_color() * emission_radiance * 1.0f / pow(distance_to_emission, 2.0f));

        // Add photon to array
        photons_temp.push_back(new Photon(irradiance, collision_pos, emission_direction));

        // Check material BRDF if we get reflected/transmitted rays
        //collision_material->BRDF
    }
}

void PhotonMapper::Start() {
    std::vector<Photon*> photons;
    emitting_triangles = scene->get_light_emitting_triangles();

    // Calculate how many photons each emitter will emit
    int photons_per_emitter = (int) (Renderer::NUM_PHOTONS / (emitting_triangles.size() + 1));
    for (std::vector<Triangle*>::iterator it = emitting_triangles.begin(); it != emitting_triangles.end(); ++it) {
        Triangle *triangle = *it;
        ColorDbl emitted_radiance = triangle->get_material()->get_color() * triangle->get_material()->get_flux();

        int emitted_photons = 0;
        while (emitted_photons < photons_per_emitter) {
            // Calculate emission position and direction from emission triangle
            vec3 emission_pos = triangle->RandomizePointOnTriangle();
            vec3 emission_direction = CosineDistributeDirection(triangle->get_normal());
            //vec3 emission_direction = normalize(vec3(((float) rand()) / (RAND_MAX) - 0.5f, ((float) rand()) / (RAND_MAX) - 0.5f,((float) rand()) / (RAND_MAX) - 0.5f));

            vec3 collision_pos, collision_normal;
            BaseMaterial* collision_material;

            if (scene->RayIntersection(emission_pos, emission_direction, collision_pos, collision_normal, collision_material)) {
                // Calculate irradiance from emitted radiance and dropoff
                float distance_to_light = length(emission_pos - collision_pos);
                ColorDbl irradiance = (collision_material->get_color() * emitted_radiance * 1.0f / pow(distance_to_light, 2.0f));
                Photon* photon = new Photon(irradiance, collision_pos, emission_direction);

                photons.push_back(photon);
                emitted_photons++;
            }
        }
    }

    BoundingBox region(vec3(-3.5, -6.5, -5.5), vec3(13, 6.5, 5.5));
    photon_tree = new PhotonOctree(region, photons);
    photon_tree->BuildTree();
}

PhotonOctree *PhotonMapper::get_photon_tree() {
    return photon_tree;
}


