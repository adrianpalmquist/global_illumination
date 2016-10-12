//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_PIXEL_H
#define TNCG15_PROJ_PIXEL_H


#include "Ray.h"
#include "ColorDbl.h"

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

#endif //TNCG15_PROJ_PIXEL_H
