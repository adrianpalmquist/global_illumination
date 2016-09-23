//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_TRIANGLE_H
#define TNCG15_PROJ_TRIANGLE_H


#include "ColorDbl.h"

class Triangle {
public:
    Triangle();
    ~Triangle();
    Triangle(vec3* _v1, vec3* _v2, vec3* _v3, ColorDbl* _clr);
    bool rayIntersection(vec3 origin, vec3 direction, vec3 &intersection_point);

    vec3 get_normal();
    vec3 get_reflection(vec3 direction, vec3 normal);
    ColorDbl* get_clr();
private:
    vec3* v0;
    vec3* v1;
    vec3* v2;
    vec3 normal;
    ColorDbl* clr;

    // Class functions
    void calculateNormal();
};

#endif //TNCG15_PROJ_TRIANGLE_H
