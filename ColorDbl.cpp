//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include "ColorDbl.h"

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

const ColorDbl& ColorDbl::operator+=(const ColorDbl& clrDbl) {
    *this = this->get_rgb() + clrDbl.get_rgb();
    return *this;
}

ColorDbl ColorDbl::operator/(const float& x) const{
    ColorDbl temp(this->get_rgb().r/x,
                  this->get_rgb().g/x,
                  this->get_rgb().b/x);
    return temp;
}