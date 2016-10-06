//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_RAYTRACER_H
#define TNCG15_PROJ_RAYTRACER_H

#include "Scene.h"
#include "Ray.h"

class RayTracer {
public:
    RayTracer();
    RayTracer(Scene* _scene);
    void StartRayTracing(Ray* base_ray);
private:
    Scene* scene;
    Ray* ray_tree;

    void TraceRay(Ray* ray);
    ColorDbl TraceShadowRays(Ray* ray, vec3 collision_point);
};

#endif //TNCG15_PROJ_RAYTRACER_H
