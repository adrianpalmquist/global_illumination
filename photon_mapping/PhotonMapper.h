//
// Created by Filip Kantedal on 2016-10-06.
//

#ifndef TNCG15_PROJ_PHOTONMAPPER_H
#define TNCG15_PROJ_PHOTONMAPPER_H


#include "PhotonKdTree.h"
#include "../glm/glm.hpp"
#include "../scene/Scene.h"
#include "PhotonOctree.h"

using namespace glm;

class PhotonMapper {
public:
    PhotonMapper();
    PhotonMapper(Scene* _scene);

    void Start();

    std::vector<Photon*> temporary_photons;

    PhotonOctree* get_photon_map();
private:
    Scene* scene;

    PhotonOctree* photon_map;

    std::vector<Triangle*> emitting_triangles;
    int emitted_photons = 0;

    // Used when distributing photons from plane emitters
    void EmitPhoton(vec3 emission_pos, vec3 emission_direction, ColorRGB emission_radiance);
    vec3 CosineDistributeDirection(vec3 normal);
};


#endif //TNCG15_PROJ_PHOTONMAPPER_H
