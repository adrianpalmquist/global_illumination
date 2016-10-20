//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_RENDERER_H
#define TNCG15_PROJ_RENDERER_H

#include "scene/Scene.h"
#include "Ray.h"
#include "Camera.h"
#include "RayTracer.h"
#include "photon_mapping/PhotonMapper.h"

class Renderer {
public:
    static const bool ANTI_ALIASING = false;
    static const int ANTI_ALIASING_SAMPLES = 4;
    static const int NUM_SHADOW_RAYS = 1;
    static const int PATHTRACING_SAMPLES = 1000;

    // Photon mapping settings
    static const int NUM_PHOTONS = 2000000;
    static constexpr float PHOTON_RADIUS = 0.2f;

    int progress_indicator = -1;

    Renderer();
    void Render();
    void CreateImage(int pathtracing_iteration);
private:
    Scene *scene;
    Camera camera;
    PhotonMapper photon_mapper;
    RayTracer ray_tracer;

    void progessIndicator(double progress);
};


#endif //TNCG15_PROJ_RENDERER_H
