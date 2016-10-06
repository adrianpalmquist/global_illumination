//
// Created by Filip Kantedal on 2016-10-06.
//

#include "PhotonMapper.h"

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
}
