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
        if (collision_material->get_material_type() == BaseMaterial::DIFFUSE) {
            // Calculate irradiance from emitted radiance and dropoff
            float distance_to_emission = length(emission_pos - collision_pos);
            ColorRGB radiance = (collision_material->BRDF(emission_direction, vec3(0), collision_normal) * emission_radiance);

            if (distance_to_emission < 0.2)
                radiance = ColorRGB(0,0,0);

            // Add photon to temporary photon map
            temporary_photons.push_back(new Photon(radiance, collision_pos, emission_direction));
            emitted_photons++;

            // Check material BRDF if we get reflected/transmitted rays
            vec3 reflected_dir, transmitted_dir;
            float radiance_distribution = 1.0;
            collision_material->PDF(emission_direction, collision_normal, reflected_dir, transmitted_dir, radiance_distribution);

            if (length(reflected_dir) != 0) {
                ColorRGB reflected_radiance = radiance; // * radiance_distribution;
                EmitPhoton(collision_pos, reflected_dir, reflected_radiance);
            }
        }
    }
}

void PhotonMapper::Start() {
    srand (time(NULL));
    const clock_t begin_time = clock();

    emitting_triangles = scene->get_light_emitting_triangles();

    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0, (int) emitting_triangles.size() - 1); // guaranteed unbiased

    while (emitted_photons < Renderer::NUM_PHOTONS) {
        // Randomize triangle to emit from
        int triangle_index = uni(rng);
        Triangle* triangle = emitting_triangles.at(triangle_index);
        ColorRGB emitted_radiance = ColorRGB(5, 5, 5); //triangle->get_material()->get_color() * triangle->get_material()->get_flux();

        // Calculate emission position and direction from emission triangle
        vec3 emission_pos = triangle->RandomizePointOnTriangle();
        vec3 emission_direction = CosineDistributeDirection(triangle->get_normal());
        //vec3 emission_direction = normalize(vec3(((float) rand()) / (RAND_MAX) - 0.5f, ((float) rand()) / (RAND_MAX) - 0.5f,((float) rand()) / (RAND_MAX) - 0.5f));

        EmitPhoton(emission_pos, emission_direction, emitted_radiance);
    }


    BoundingBox region(vec3(-3.5, -6.5, -5.5), vec3(13, 6.5, 5.5));

    // Build map for photons
    photon_map = new PhotonOctree(region, temporary_photons, 0);
    photon_map->BuildTree();
    //photon_map->displayTree();
    std::cout << "Photon mapping finished, photons emitted: " << emitted_photons << ", time elapsed: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << "s" << std::endl;
}

PhotonOctree *PhotonMapper::get_photon_map() {
    return photon_map;
}


