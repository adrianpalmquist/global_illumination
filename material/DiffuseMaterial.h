//
// Created by Filip Kantedal on 27/09/16.
//

#ifndef TNCG15_PROJ_DIFFUSEMATERIAL_H
#define TNCG15_PROJ_DIFFUSEMATERIAL_H


#include "BaseMaterial.h"

class DiffuseMaterial : public BaseMaterial {
public:
    DiffuseMaterial(ColorDbl _color) : BaseMaterial(_color) { };

    void BRDF(vec3 ray_direction, vec3 collision_normal, vec3 &reflected_dir, vec3 &transmitted_dir, float &radiance_dist);
    int get_material_type() { return BaseMaterial::DIFFUSE; }
};


#endif //TNCG15_PROJ_DIFFUSEMATERIAL_H
