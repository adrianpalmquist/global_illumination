//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_PIXEL_H
#define TNCG15_PROJ_PIXEL_H


#include "Ray.h"
#include "ColorRGB.h"

class Pixel {
public:
    Pixel();
    ~Pixel();
    Ray * get_base_ray() const;
    ColorRGB get_clr() const;
    void set_clr(ColorRGB vec);
private:
    ColorRGB clr;
    Ray * base_ray;
};

#endif //TNCG15_PROJ_PIXEL_H
