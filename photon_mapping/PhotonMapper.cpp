//
// Created by Filip Kantedal on 2016-10-06.
//

#include <iostream>
#include <random>
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

void PhotonMapper::EmitPhoton(vec3 emission_pos, vec3 emission_direction, ColorRGB emission_radiance) {
    vec3 collision_pos, collision_normal;
    BaseMaterial* collision_material;

    Ray ray(emission_pos, emission_direction, 0);
    if (scene->RayIntersection(ray, collision_pos, collision_normal, collision_material)) {
        if (collision_material->get_material_type() == BaseMaterial::DIFFUSE || collision_material->get_material_type() == BaseMaterial::OREN_NAYAR) {
            // Check material BRDF if we get reflected/transmitted rays
            vec3 reflected_dir, transmitted_dir = vec3(0.0);
            float radiance_distribution = 1.0;

            collision_material->PDF(emission_direction, collision_normal, reflected_dir, transmitted_dir, radiance_distribution);

            ColorRGB radiance = collision_material->BRDF(emission_direction, reflected_dir, collision_normal) * emission_radiance;

            // Add photon to temporary photon map
            temporary_photons.push_back(new Photon(radiance, collision_pos, emission_direction));
            emitted_photons++;

            if (length(reflected_dir) != 0 && !isnan(length(reflected_dir))) {
                EmitPhoton(collision_pos + reflected_dir * 0.01f, reflected_dir, radiance);
            }
        }
    }
}

void PhotonMapper::EmitCausticsPhoton(vec3 emission_pos, vec3 emission_direction, ColorRGB emission_radiance, bool arriving_from_transmission) {
    vec3 collision_pos, collision_normal;
    BaseMaterial* collision_material;

    Ray ray(emission_pos, emission_direction, 0);
    if (scene->RayIntersection(ray, collision_pos, collision_normal, collision_material)) {
        if (arriving_from_transmission && (collision_material->get_material_type() == BaseMaterial::DIFFUSE || collision_material->get_material_type() == BaseMaterial::OREN_NAYAR)) {
            // Check material BRDF if we get reflected/transmitted rays
            vec3 reflected_dir, transmitted_dir = vec3(0.0);
            float radiance_distribution = 1.0;

            collision_material->PDF(emission_direction, collision_normal, reflected_dir, transmitted_dir, radiance_distribution);

            ColorRGB radiance = collision_material->BRDF(emission_direction, reflected_dir, collision_normal) * emission_radiance;

            // Add photon to temporary photon map
            temporary_caustics_photons.push_back(new Photon(radiance, collision_pos, emission_direction));
            emitted_caustics_photons++;
        }

        if (collision_material->get_material_type() == BaseMaterial::TRANSMITTING) {
            // Check material BRDF if we get reflected/transmitted rays
            vec3 reflected_dir, transmitted_dir = vec3(0.0);
            float radiance_distribution = 1.0;

            collision_material->PDF(emission_direction, collision_normal, reflected_dir, transmitted_dir, radiance_distribution);
            ColorRGB radiance = collision_material->BRDF(emission_direction, reflected_dir, collision_normal) * emission_radiance;

            if (length(transmitted_dir) != 0 && !isnan(length(transmitted_dir))) {
                EmitCausticsPhoton(collision_pos + transmitted_dir * 0.01f, transmitted_dir, radiance, true);
            }
        }
    }
}

void PhotonMapper::Start() {
    srand (time(NULL));
    const clock_t begin_time = clock();

    std::vector<Object3d> emitting_objects = scene->get_light_emitting_objects();
    for (int i = 0; i < emitting_objects.size(); i++) {
        for (int j = 0; j < emitting_objects.at(i).get_triangles().size(); j++) {
            emitting_triangles.push_back(emitting_objects.at(i).get_triangles().at(j));
        }
    }

    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0, (int) emitting_triangles.size() - 1); // guaranteed unbiased

    while (emitted_photons < Renderer::NUM_PHOTONS) {
        // Randomize triangle to emit from
        int triangle_index = uni(rng);
        Triangle* triangle = emitting_triangles.at(triangle_index);
        ColorRGB emitted_radiance = ColorRGB(500, 500, 500); //triangle->get_material()->get_color() * triangle->get_material()->get_flux();

        // Calculate emission position and direction from emission triangle
        vec3 emission_pos = triangle->RandomizePointOnTriangle();
        vec3 emission_direction = CosineDistributeDirection(triangle->get_normal());

        EmitPhoton(emission_pos, emission_direction, emitted_radiance);
    }

    while (emitted_caustics_photons < Renderer::NUM_CAUSTICS_PHOTONS) {
        // Randomize triangle to emit from
        int triangle_index = uni(rng);
        Triangle* triangle = emitting_triangles.at(triangle_index);
        ColorRGB emitted_radiance = ColorRGB(500, 500, 500); //triangle->get_material()->get_color() * triangle->get_material()->get_flux();

        // Calculate emission position and direction from emission triangle
        vec3 emission_pos = triangle->RandomizePointOnTriangle();
        vec3 emission_direction = CosineDistributeDirection(triangle->get_normal());
        //vec3 emission_direction = normalize(vec3(((float) rand()) / (RAND_MAX) - 0.5f, ((float) rand()) / (RAND_MAX) - 0.5f,((float) rand()) / (RAND_MAX) - 0.5f));

        EmitCausticsPhoton(emission_pos, emission_direction, emitted_radiance, false);
    }


    BoundingBox region(vec3(-3.5, -6.5, -5.5), vec3(13, 6.5, 5.5));

    // Build map for photons
    photon_map = new PhotonOctree(region, temporary_photons, 0);
    photon_map->BuildTree();

    caustics_photon_map = new PhotonOctree(region, temporary_caustics_photons, 0);
    caustics_photon_map->BuildTree();

    std::cout << "Photon mapping finished, photons emitted: " << emitted_photons << ", time elapsed: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << "s" << std::endl;
}

PhotonOctree *PhotonMapper::get_photon_map() {
    return photon_map;
}

PhotonOctree *PhotonMapper::get_caustics_photon_map() {
    return caustics_photon_map;
}


