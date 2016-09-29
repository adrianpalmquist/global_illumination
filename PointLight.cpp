//
// Created by Filip Kantedal on 26/09/16.
//

#include "PointLight.h"

PointLight::PointLight(vec3 _position) {
    position = _position;
}

PointLight::PointLight() {
    position = vec3(0.0, 0.0, 0.0);
}
