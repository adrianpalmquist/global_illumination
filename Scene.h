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

    bool RayIntersection(vec3 start_point, vec3 direction, vec3 &collision_pos, vec3 &collision_normal, BaseMaterial &collision_material, bool &reflect);
    bool RayIntersection(vec3 start_point, vec3 direction, vec3 &collision_pos);
    void PrepareForRender();

    std::vector<Triangle*> get_triangles();
    std::vector<Triangle*> get_light_emitting_triangles();
    std::vector<Sphere*> get_spheres();
private:
    std::vector<Triangle*> triangles;
    std::vector<Triangle*> light_emitting_triangles; // Calucated before render
    std::vector<Sphere*> spheres;

    void CreateDefaultScene();
};

#endif //TNCG15_PROJ_SCENE_H
