//
// Created by Filip Kantedal on 13/09/16.
//

#pragma once

#include "ColorDbl.h"
#include "Ray.h"

class Pixel {
public:
    Pixel();
    ~Pixel();
    Ray * get_base_ray() const;
    ColorDbl get_clr() const;
    void set_clr(ColorDbl vec);
private:
    ColorDbl clr;
    Ray * base_ray;
};

Ray * Pixel::get_base_ray() const {
    return base_ray;
}

ColorDbl Pixel::get_clr() const{
    return clr;
}

void Pixel::set_clr(ColorDbl _clr) {
    clr = _clr;
}

Pixel::Pixel() {

}

Pixel::~Pixel() {
}



