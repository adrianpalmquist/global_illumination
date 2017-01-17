//
// Created by Filip Kantedal on 2016-10-06.
//

#include "Photon.h"

const bool Photon::operator==(Photon& photon) {
    if (photon.get_position().x == position.x && photon.get_position().x == position.x && photon.get_position().z == position.z
        && photon.get_radiance().get_rgb().r == radiance.get_rgb().r && photon.get_radiance().get_rgb().g == radiance.get_rgb().g
        && photon.get_radiance().get_rgb().b == radiance.get_rgb().b)
        return true;
    return false;
}