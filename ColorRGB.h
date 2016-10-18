//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_COLORDBL_H
#define TNCG15_PROJ_COLORDBL_H

#include "glm/glm.hpp"

using namespace glm;

class ColorRGB {
public:
    ColorRGB() {};

    ColorRGB(vec3 vec) : rgb(vec) {}
    ColorRGB(float r, float g, float b);
    const ColorRGB& operator+=(const ColorRGB& clr_dbl);
    ColorRGB operator/(const float& x) const;
    ColorRGB operator*(const float& x) const;
    ColorRGB operator*(const ColorRGB& clr) const;
    friend ColorRGB operator+(ColorRGB lhs, const ColorRGB& rhs);

    const vec3 get_rgb() const;
    void set_rgb(vec3 _rgb);
    const float get_clr_magnitude() const;
private:
    vec3 rgb;
};

#endif //TNCG15_PROJ_COLORDBL_H

