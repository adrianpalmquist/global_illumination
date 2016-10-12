//
// Created by Filip Kantedal on 27/09/16.
//

#ifndef TNCG15_PROJ_BASEMATERIAL_H
#define TNCG15_PROJ_BASEMATERIAL_H


#include "../ColorDbl.h"
#include "../Ray.h"

class BaseMaterial {
public:
    static const int DIFFUSE = 0;
    static const int SPECULAR = 1;
    static const int TRANSMITTING = 2;
    static const int EMITTING = 3;

    BaseMaterial();
    explicit BaseMaterial(ColorDbl _color);

    const ColorDbl get_color() const;
    void set_color(ColorDbl _color);

    const bool is_emitting_light();
    void enable_light_emission();

    void set_light_color(ColorDbl _light_color);
    ColorDbl get_light_color();

    void set_flux(float _flux);
    float get_flux();

    const bool is_transparent();
    void set_transparent();

    virtual void BRDF(Ray* ray, vec3 collision_normal)=0;
private:
    ColorDbl color;
    float opacity;
    float specularity;

    // Light mission attributes
    bool light_emission_enabled;
    bool transparent;
    ColorDbl light_color;
    float flux;
};


#endif //TNCG15_PROJ_BASEMATERIAL_H
