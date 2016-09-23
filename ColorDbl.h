//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_COLORDBL_H
#define TNCG15_PROJ_COLORDBL_H

#include "glm/glm.hpp"

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
    dvec3 rgb;
};

#endif //TNCG15_PROJ_COLORDBL_H
