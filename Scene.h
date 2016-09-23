//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_SCENE_H
#define TNCG15_PROJ_SCENE_H


#include <vector>
#include "Triangle.h"
#include "Sphere.h"

class Scene {
public:
    Scene();
    ~Scene();

    std::vector<Triangle*> get_triangles();
    std::vector<Sphere*> get_spheres();
private:
    std::vector<Triangle*> triangles;
    std::vector<Sphere*> spheres;

    void CreateDefaultScene();
};

#endif //TNCG15_PROJ_SCENE_H
