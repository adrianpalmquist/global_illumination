//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_RAY_H
#define TNCG15_PROJ_RAY_H


#include <vector>
#include "glm/glm.hpp"
#include "ColorRGB.h"
//#include "Scene.h"

using namespace glm;

class Ray {
public:
    Ray(vec3 _start_point, vec3 _direction, int _ray_iteration);
    Ray();

    ~Ray();

    int get_ray_iterations();
    vec3 get_start_point();
    vec3 get_direction();

    vec3 get_end_point();
    void set_end_point(vec3 _end_point);

    ColorRGB get_ray_color();
    void set_ray_color(ColorRGB _ray_clr);
    void add_ray_color(ColorRGB _ray_clr);

    float get_radiance_distribution();
    void set_radiance_distribution(float _radiance_distribution);

    std::vector<Ray*> get_shadow_rays();
    void add_shadow_ray(Ray* shadow_ray);

private:
    std::vector<Ray*> shadow_rays;

    vec3 start_point;
    vec3 end_point;
    vec3 direction;
    ColorRGB ray_clr;
    float radiance_distribution;

    int ray_iteration;
};

#endif //TNCG15_PROJ_RAY_H

