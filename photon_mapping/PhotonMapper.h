//
// Created by Filip Kantedal on 2016-10-06.
//

#ifndef TNCG15_PROJ_PHOTONMAPPER_H
#define TNCG15_PROJ_PHOTONMAPPER_H


#include "PhotonKdTree.h"
#include "../glm/glm.hpp"
#include "../Scene.h"

using namespace glm;

class PhotonMapper {
public:
    PhotonMapper(Scene* _scene);

    void Start();
private:
    Scene* scene;
    PhotonKdTree* photon_tree;

    // Used when distributing photons from plane emitters
    vec3 CosineDistributeDirection(vec3 normal);
};


#endif //TNCG15_PROJ_PHOTONMAPPER_H
