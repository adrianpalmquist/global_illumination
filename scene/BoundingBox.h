//
// Created by Filip Kantedal on 2016-10-11.
//

#ifndef TNCG15_PROJ_BOUNDINGBOX_H
#define TNCG15_PROJ_BOUNDINGBOX_H


#include "../glm/vec3.hpp"
#include "../Ray.h"

using namespace glm;

class BoundingBox {
public:
    BoundingBox();
    BoundingBox(vec3 _min, vec3 _max);

    bool Contains(vec3 p);
    bool ConainsPoint(vec3 p);
    bool RayIntersection(Ray ray);

    const vec3 get_min();
    const vec3 get_max();
private:
    // Bounding box corners
    vec3 min;
    vec3 max;
    vec3 v1, v2, v3, v4, v5, v6, v7, v8;
};


#endif //TNCG15_PROJ_BOUNDINGBOX_H
