//
// Created by Filip Kantedal on 27/09/16.
//

#ifndef TNCG15_PROJ_BASEMATERIAL_H
#define TNCG15_PROJ_BASEMATERIAL_H


#include "../ColorRGB.h"
#include "../Ray.h"

class BaseMaterial {
public:
    static const int DIFFUSE = 0;
    static const int SPECULAR = 1;
    static const int TRANSMITTING = 2;
    static const int EMISSION = 3;
    static const int OREN_NAYAR = 4;

    int type;

    BaseMaterial();
    explicit BaseMaterial(ColorRGB _color);

    const ColorRGB get_color() const;
    void set_color(ColorRGB _color);

    const bool is_emitting_light();
    void enable_light_emission();

    void set_light_color(ColorRGB _light_color);
    ColorRGB get_light_color();

    void set_flux(float _flux);
    float get_flux();

    const bool is_transparent();
    void set_transparent();

    virtual ColorRGB BRDF(vec3 incoming_direction, vec3 outgoing_direction, vec3 collision_normal) = 0;
    virtual void PDF(vec3 ray_direction, vec3 collision_normal, vec3 &reflected_dir, vec3 &transmitted_dir, float &radiance_dist) = 0;

    virtual int get_material_type() = 0;
private:
    ColorRGB color;
    float opacity;
    float specularity;

    // Light mission attributes
    bool light_emission_enabled;
    bool transparent;
    ColorRGB light_color;
    float flux;
};


#endif //TNCG15_PROJ_BASEMATERIAL_H
