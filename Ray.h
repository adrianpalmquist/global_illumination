//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_RAY_H
#define TNCG15_PROJ_RAY_H


#include "glm/glm.hpp"
#include "Scene.h"

using namespace glm;

class Ray {
public:
    Ray(Ray* _parent_ray, vec3 *_start_point, vec3 _direction, int _ray_iteration);
    ~Ray();

    int get_ray_iterations();
    vec3* get_start_point();
    vec3 get_direction();

    vec3* get_end_point();
    void set_end_point(vec3* _end_point);

    ColorDbl get_ray_color();
    void set_ray_color(ColorDbl _ray_clr);

    Ray* get_transmitted_ray();
    void set_transmitted_ray(Ray* _transmitted_ray);

    Ray* get_reflected_ray();
    void set_reflected_ray(Ray* _reflected_ray);

    float get_radiance_distribution();
    void set_radiance_distribution(float _radiance_distribution);

    std::vector<Ray*> get_shadow_rays();
    void add_shadow_ray(Ray* shadow_ray);

private:
    Ray* parent_ray;
    Ray* reflected_ray;
    Ray* transmitted_ray;
    std::vector<Ray*> shadow_rays;

    vec3 * start_point;
    vec3 * end_point;
    vec3 direction;
    ColorDbl ray_clr;
    float radiance_distribution;

    int ray_iteration;
};


#endif //TNCG15_PROJ_RAY_H
