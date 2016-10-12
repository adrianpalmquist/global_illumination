//
// Created by Filip Kantedal on 27/09/16.
//

#include "BaseMaterial.h"

BaseMaterial::BaseMaterial() {
    color = ColorDbl(0.0, 0.0, 0.0);
    specularity = 0.0f;
    opacity = 0.0f;
    light_emission_enabled = false;
    flux = 0.0f;
    light_color = ColorDbl(0.0, 0.0, 0.0);
    transparent = false;
}

BaseMaterial::BaseMaterial(ColorDbl _color) {
    color = _color;
    specularity = 0.0f;
    opacity = 0.0f;
    light_emission_enabled = false;
    flux = 0.0f;
    light_color = ColorDbl(0.0, 0.0, 0.0);
    transparent = false;
}

const ColorDbl BaseMaterial::get_color() const {
    return color;
}

void BaseMaterial::set_color(ColorDbl _color) {
    color = _color;
}

const bool BaseMaterial::is_emitting_light() {
    return light_emission_enabled;
}

void BaseMaterial::enable_light_emission() {
    light_emission_enabled = true;
}

void BaseMaterial::set_light_color(ColorDbl _light_color) {
    light_color = _light_color;
}

void BaseMaterial::set_flux(float _light_power) {
    flux = _light_power;
}

float BaseMaterial::get_flux() {
    return flux;
}

ColorDbl BaseMaterial::get_light_color() {
    return light_color;
}

const bool BaseMaterial::is_transparent() {
    return transparent;
}

void BaseMaterial::set_transparent() {
    transparent = true;
}

