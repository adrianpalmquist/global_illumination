//
// Created by Filip Kantedal on 2016-10-06.
//

#ifndef TNCG15_PROJ_PHOTON_H
#define TNCG15_PROJ_PHOTON_H


#include "../ColorRGB.h"

class Photon {
public:
    Photon(ColorRGB _radiance, vec3 _position, vec3 _direction): radiance(_radiance), position(_position), direction(_direction) {};

    vec3 get_position() { return position; }
    ColorRGB get_radiance() { return radiance; }
    vec3 get_direction() { return direction; }

    const bool operator==(Photon& photon);
private:
    vec3 position;
    vec3 direction;
    ColorRGB radiance;
};


#endif //TNCG15_PROJ_PHOTON_H
