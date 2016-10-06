//
// Created by Filip Kantedal on 27/09/16.
//

#ifndef TNCG15_PROJ_SPECULARMATERIAL_H
#define TNCG15_PROJ_SPECULARMATERIAL_H


#include "BaseMaterial.h"

class SpecularMaterial : public BaseMaterial {
public:
    SpecularMaterial(ColorDbl _color) : BaseMaterial(_color) { };

    virtual void BRDF(Ray* ray, vec3 collision_normal);

private:
    vec3 ReflectRay(Ray* ray, vec3 collision_normal);
    void TransmitRay(Ray* ray, vec3 collision_normal, vec3 reflected_direction);
};


#endif //TNCG15_PROJ_SPECULARMATERIAL_H
