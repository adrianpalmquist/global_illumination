//
// Created by Filip Kantedal on 23/09/16.
//

#include "Triangle.h"


Triangle::Triangle() {}

Triangle::~Triangle() {}

Triangle::Triangle(vec3* _v1, vec3* _v2, vec3* _v3) : v0(_v1), v1(_v2), v2(_v3) {
    calculateNormal();
    calculate_triangle_area();
}

// Calculates normal from vertices
void Triangle::calculateNormal() {
    vec3 baseVec1 = *v1 - *v0;
    vec3 baseVec2 = *v2 - *v0;
    vec3 new_normal = normalize(cross(baseVec1, baseVec2));

    normal = new_normal;
}

// Returns normal direction
vec3 Triangle::get_normal() { return normal; }

bool Triangle::RayIntersection(vec3 origin, vec3 direction, vec3 &intersection_point) {
    static const float EPS = 0.0001;

    vec3 v0v1(*v1 - *v0);
    vec3 v0v2(*v2 - *v0);

    //Begin calculating determinant - also used to calculate u parameter
    vec3 P(cross(direction, v0v2));
    float det = dot(v0v1, P);

    if(det > -EPS && det < EPS) return false;
    float inv_det = 1.0f / det;

    //Distance from vertex1 to ray origin
    vec3 T = origin - *v0;
    float u = dot(T, P);
    if(u < 0.0f || u > det) return false;

    vec3 Q = cross(T, v0v1);

    float v = dot(direction, Q);
    if(v < 0.0f || u+v > det) return false;

    float t = dot(v0v2, Q);

    if(t > EPS) {
        intersection_point = origin + inv_det*t*direction;
        return true;
    }

    return false;
}

vec3 Triangle::get_reflection(vec3 direction, vec3 normal) {
    // float angle = acos(dot(direction,normal));
    vec3 reflected = normalize(direction - 2 * dot(direction,normal) * normal);

    return reflected;
}

vec3 Triangle::BarycentricToCartesian(float u, float v) {
    return (1 - u - v) * (*v0) + u * (*v1) + v * (*v2);
}

vec3 Triangle::RandomizePointOnTriangle() {
//    float u = ((float) rand() / (RAND_MAX));
//    float v = (1.0f - u) * ((float) rand() / (RAND_MAX));
//    return BarycentricToCartesian(u, v);
    float u, v;

    while (true) {
        // Randomize two points on the emission triangle using Baycentric coordinate
        u = ((float) rand() / (RAND_MAX));
        v = ((float) rand() / (RAND_MAX));

        // Check so that the coordinate sum is less than 1
        if (u + v < 1) {
            return BarycentricToCartesian(u, v);
        }
    }
}

void Triangle::calculate_triangle_area() {
    vec3 edge1 = *v1 - *v0;
    vec3 edge2 = *v2 - *v0;
    triangle_area = 0.5f * length(cross(edge1, edge2));
}

float Triangle::get_triangle_area() {
    return triangle_area;
}
