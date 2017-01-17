//
// Created by Filip Kantedal on 27/09/16.
//

#ifndef TNCG15_PROJ_SPECULARMATERIAL_H
#define TNCG15_PROJ_SPECULARMATERIAL_H


#include "BaseMaterial.h"

class SpecularMaterial : public BaseMaterial {
public:
    SpecularMaterial(ColorRGB _color) : BaseMaterial(_color) { };

    ColorRGB BRDF(vec3 incoming_direction, vec3 outgoing_direction, vec3 collision_normal);
    void PDF(vec3 ray_direction, vec3 collision_normal, vec3 &reflected_dir, vec3 &transmitted_dir, float &radiance_dist);

    int get_material_type() { return BaseMaterial::SPECULAR; }
private:
    void ReflectRay(vec3 direction, vec3 collision_normal, vec3& reflected_direction);
    void TransmitRay(vec3 direction, vec3 collision_normal, vec3 reflected_direction, float &radiance_dist, vec3 &transmitted_direction);
};


#endif //TNCG15_PROJ_SPECULARMATERIAL_H
