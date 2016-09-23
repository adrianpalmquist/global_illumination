//
// Created by Adrian Palmquist on 2016-09-23.
//

#pragma once

#include "ColorDbl.h"

class Material {
public:
    Material();
    Material(int _material, ColorDbl _clr, bool incoming);
    ~Material();

private:
    static const int DIFFUSE = 0;
    static const int SPECULAR = 1;
    static const int TRANSPARANT = 2;
    float n1 = 1.000293; // air
    float n2 = 1.52; // glass
    int material;
    ColorDbl clr;

};

Material::Material() {

}

Material::Material(int _material, ColorDbl _clr, bool incoming) : material(_material), clr(_clr) {
    if (!incoming) {
        n2 = 1.000293;
        n1 = 1.52;
    }
}

Material::~Material() {

}
