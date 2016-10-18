//
// Created by Filip K on 18/10/16.
//

#ifndef TNCG15_PROJ_EMISSIONMATERIAL_H
#define TNCG15_PROJ_EMISSIONMATERIAL_H


#include "BaseMaterial.h"

class EmissionMaterial : public BaseMaterial {
public:
    EmissionMaterial(ColorRGB _color, float _flux) : BaseMaterial(_color) {
        set_flux(_flux);
    };

    ColorRGB BRDF(vec3 incoming_direction, vec3 outgoing_direction, vec3 collision_normal) {
        return get_color() * get_flux() * (1.0f / (float) M_PI *  dot(normalize(-incoming_direction), normalize(collision_normal)));
    };

    void PDF(vec3 ray_direction, vec3 collision_normal, vec3 &reflected_dir, vec3 &transmitted_dir, float &radiance_dist) { };

    int get_material_type() { return BaseMaterial::EMISSION; }
};


#endif //TNCG15_PROJ_EMISSIONMATERIAL_H
