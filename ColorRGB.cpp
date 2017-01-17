//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include "ColorRGB.h"

ColorRGB::ColorRGB(float r, float g, float b) {
    rgb.r = r;
    rgb.g = g;
    rgb.b = b;
}

const vec3 ColorRGB::get_rgb() const {
    return rgb;
}

const float ColorRGB::get_clr_magnitude() const {
    return sqrtf(pow(rgb.r, 2.0) + pow(rgb.g, 2.0) + pow(rgb.b, 2.0));
}

void ColorRGB::set_rgb(vec3 _rgb) {
    rgb = _rgb;
}

const ColorRGB& ColorRGB::operator+=(const ColorRGB& clrDbl) {
    *this = this->get_rgb() + clrDbl.get_rgb();
    return *this;
}

ColorRGB ColorRGB::operator/(const float& x) const{
    ColorRGB clr(this->get_rgb().r/x,
                 this->get_rgb().g/x,
                 this->get_rgb().b/x);
    return clr;
}

ColorRGB ColorRGB::operator*(const float &factor) const {
    ColorRGB clr(this->get_rgb().r * factor,
                 this->get_rgb().g * factor,
                 this->get_rgb().b * factor);
    return clr;
}

ColorRGB ColorRGB::operator*(const ColorRGB &clr) const {
    ColorRGB color(this->get_rgb().r * clr.get_rgb().r,
                   this->get_rgb().g * clr.get_rgb().g,
                   this->get_rgb().b * clr.get_rgb().b);
    return color;
}

ColorRGB operator+(ColorRGB lhs, const ColorRGB &rhs) {
    lhs += rhs;
    return lhs;
}
