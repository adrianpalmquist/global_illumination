//
// Created by Filip Kantedal on 27/09/16.
//

#ifndef TNCG15_PROJ_DIFFUSEMATERIAL_H
#define TNCG15_PROJ_DIFFUSEMATERIAL_H


#include "BaseMaterial.h"

class DiffuseMaterial : public BaseMaterial {
public:
    DiffuseMaterial(ColorDbl _color) : BaseMaterial(_color) { };

    virtual void BRDF(Ray* ray, vec3 collision_normal);
};


#endif //TNCG15_PROJ_DIFFUSEMATERIAL_H
