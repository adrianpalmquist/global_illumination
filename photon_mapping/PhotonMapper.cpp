//
// Created by Filip Kantedal on 2016-10-06.
//

#include "PhotonMapper.h"
#include "../Renderer.h"

PhotonMapper::PhotonMapper(Scene *_scene) : scene(_scene) {}

vec3 PhotonMapper::CosineDistributeDirection(vec3 normal) {
    float sin_theta = (float) sqrt(rand() / RAND_MAX);
    float cos_theta = (float) sqrt(1.0 - sin_theta * sin_theta);

    // Random in plane angle;
    float psi = (float) (rand() / RAND_MAX * 2 * M_PI);

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

void PhotonMapper::Start() {
    std::vector<Photon> photons;
    emitting_triangles = scene->get_light_emitting_triangles();

    // Calculate how many photons each emitter will emit
    int photons_per_emitter = (int) (Renderer::NUM_PHOTONS / (emitting_triangles.size() + 1));
    for (std::vector<Triangle*>::iterator it = emitting_triangles.begin(); it != emitting_triangles.end(); ++it) {
        Triangle *triangle = *it;

        int emitted_photons = 0;
        while (emitted_photons < photons_per_emitter) {
            vec3 emission_pos = triangle->RandomizePointOnTriangle();
            vec3 emission_direction = CosineDistributeDirection(triangle->get_normal());

            vec3 collision_pos;
            scene->RayIntersection(emission_pos, emission_direction, collision_pos);
            emitted_photons++;
        }
    }

}
