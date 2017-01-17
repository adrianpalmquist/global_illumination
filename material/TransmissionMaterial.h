//
// Created by Filip K on 18/10/16.
//

#ifndef TNCG15_PROJ_TRANSMISSIONMATERIAL_H
#define TNCG15_PROJ_TRANSMISSIONMATERIAL_H

#include "BaseMaterial.h"

class TransmissionMaterial : public BaseMaterial {
public:
    TransmissionMaterial(ColorRGB _color) : BaseMaterial(_color) { };

    ColorRGB BRDF(vec3 incoming_direction, vec3 outgoing_direction, vec3 collision_normal);
    void PDF(vec3 ray_direction, vec3 collision_normal, vec3 &reflected_dir, vec3 &transmitted_dir, float &radiance_dist);

    int get_material_type() { return BaseMaterial::TRANSMITTING; }
private:
    void ReflectRay(vec3 direction, vec3 collision_normal, vec3& reflected_direction);
    void TransmitRay(vec3 direction, vec3 collision_normal, vec3 reflected_direction, float &radiance_dist, bool incoming_ray, vec3 &transmitted_direction);
};


#endif //TNCG15_PROJ_TRANSMISSIONMATERIAL_H
