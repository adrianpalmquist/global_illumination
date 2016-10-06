//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_RENDERER_H
#define TNCG15_PROJ_RENDERER_H

#include "Scene.h"
#include "Ray.h"
#include "Camera.h"
#include "RayTracer.h"

class Renderer {
public:
    static const bool ANTI_ALIASING = false;
    static const int ANTI_ALIASING_SAMPLES = 4;
    static const int NUM_SHADOW_RAYS = 5;
    static const int PATH_TRACING_MAX_SAMPLES = 1000;

    Renderer();
    void Render();
    void CreateImage(int pathtracing_iteration);
private:
    Scene *scene;
    Camera camera;
    RayTracer ray_tracer;

    ColorDbl ColorFromRayTree(Ray *parentRay);
};


#endif //TNCG15_PROJ_RENDERER_H
