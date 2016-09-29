//
// Created by Filip Kantedal on 26/09/16.
//

#ifndef TNCG15_PROJ_POINTLIGHT_H
#define TNCG15_PROJ_POINTLIGHT_H

#include "glm/glm.hpp"

using namespace glm;

class PointLight {
public:
    PointLight();
    PointLight(vec3 _position);
private:
    vec3 position;
};


#endif //TNCG15_PROJ_POINTLIGHT_H
