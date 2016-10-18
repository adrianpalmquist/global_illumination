//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_TRIANGLE_H
#define TNCG15_PROJ_TRIANGLE_H


#include "../ColorRGB.h"
#include "../material/BaseMaterial.h"

class Triangle {
public:
    Triangle();
    ~Triangle();
    Triangle(vec3* _v1, vec3* _v2, vec3* _v3, BaseMaterial* _material);

    bool RayIntersection(vec3 origin, vec3 direction, vec3 &intersection_point);
    vec3 BarycentricToCartesian(float u, float v);

    vec3 get_normal();
    vec3 get_reflection(vec3 direction, vec3 normal);
    BaseMaterial* get_material();

    vec3* get_v0() { return v0; }
    vec3* get_v1() { return v1; }
    vec3* get_v2() { return v2; }

    vec3 RandomizePointOnTriangle();
private:
    vec3* v0;
    vec3* v1;
    vec3* v2;
    vec3 normal;
    BaseMaterial* material;

    void calculateNormal();
};

#endif //TNCG15_PROJ_TRIANGLE_H
