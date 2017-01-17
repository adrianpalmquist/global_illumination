//
// Created by Filip Kantedal on 23/09/16.
//

#include "Pixel.h"

Ray * Pixel::get_base_ray() const {
    return base_ray;
}

ColorRGB Pixel::get_clr() const {
    return clr;
}

void Pixel::set_clr(ColorRGB _clr) {
    clr = _clr;
}

Pixel::Pixel() {

}

Pixel::~Pixel() {
}


