//
// Created by Filip Kantedal on 06/09/16.
//
#pragma once

#include <math.h>
#include "glm/vec3.hpp"

using namespace glm;

class ColorDbl {
public:
    ColorDbl() {};

    ColorDbl(dvec3 vec) : rgb(vec) {}
    ColorDbl(double r, double g, double b);

    const dvec3 get_rgb() const;
    void set_rgb(dvec3 _rgb);
    const double get_clr_magnitude() const;
private:
    dvec3 rgb = dvec3(0.0);
};

ColorDbl::ColorDbl(double r, double g, double b) {
    rgb.r = r;
    rgb.g = g;
    rgb.b = b;
}

const dvec3 ColorDbl::get_rgb() const {
    return rgb;
}

const double ColorDbl::get_clr_magnitude() const {
    return sqrt(pow(rgb.r, 2.0) + pow(rgb.g, 2.0) + pow(rgb.b, 2.0));
}

void ColorDbl::set_rgb(dvec3 _rgb) {
    rgb = _rgb;
}

