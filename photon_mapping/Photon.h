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

    void mark_deletion() { should_be_deleted = true; }
    void restore_deletion() { should_be_deleted = false; }
    bool should_delete() { return should_be_deleted; }
private:
    vec3 position;
    vec3 direction;
    ColorRGB radiance;
    bool should_be_deleted = false;
};


#endif //TNCG15_PROJ_PHOTON_H
