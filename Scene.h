//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_SCENE_H
#define TNCG15_PROJ_SCENE_H


#include <vector>
#include "Triangle.h"
#include "Sphere.h"

class Ray; // Forward declaration to avoid circular referencing

class Scene {
public:
    Scene();
    ~Scene();

    bool RayIntersection(Ray* ray, vec3 &intersection_point, vec3 &collision_normal, bool &reflect);

    std::vector<Triangle*> get_triangles();
    std::vector<Sphere*> get_spheres();
private:
    std::vector<Triangle*> triangles;
    std::vector<Sphere*> spheres;

    void CreateDefaultScene();
};

#endif //TNCG15_PROJ_SCENE_H
