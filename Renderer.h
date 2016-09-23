//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_RENDERER_H
#define TNCG15_PROJ_RENDERER_H

#include "Scene.h"
#include "Ray.h"
#include "Camera.h"

class Renderer {
public:
    Renderer();
    void Render();
    void CreateImage();
private:
    Scene *scene;
    Camera camera;

    ColorDbl ColorFromRayTree(Ray *parentRay);
};

#endif //TNCG15_PROJ_RENDERER_H
