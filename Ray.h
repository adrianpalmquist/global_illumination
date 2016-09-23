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
    Ray(vec3 *_start_point, vec3 _direction, int _ray_iteration);
    ~Ray();

    void TraceRay(Scene* scene);
    void CalculateClr(Scene* scene);

    int get_ray_iterations();
    Ray* get_reflected_ray();
    Ray* get_transmitted_ray();
    void set_radiance_distribution(double _radiance_distribution);
    ColorDbl get_color();
private:
    vec3 * start_point;
    vec3 * end_point;
    vec3 direction;
    ColorDbl clr;
    int ray_iteration;

    float n1;
    float n2;

    Ray* reflected_ray;
    Ray* transmitted_ray;

    bool ReflectRay(vec3 collision_normal, vec3 in_direction, vec3 &out_direction);
    bool TransmitRay(vec3 collision_normal, vec3 in_direction, bool going_in, vec3 &out_direction, vec3 reflected_direction, double &radiance_distribution);
};


#endif //TNCG15_PROJ_RAY_H
