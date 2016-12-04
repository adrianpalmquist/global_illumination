//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_SCENE_H
#define TNCG15_PROJ_SCENE_H


#include <vector>
#include "Triangle.h"
#include "../Sphere.h"
#include "Object3d.h"
#include "../Ray.h"
#include "../material/BaseMaterial.h"

// Forward declaration to avoid circular referencing
class Ray;

class Scene {
public:
    Scene();
    ~Scene();

    bool RayIntersection(Ray ray, vec3 &collision_pos, vec3 &collision_normal, BaseMaterial *&collision_material);
    bool RayIntersection(Ray ray, vec3 &collision_pos);
    void PrepareForRender();

    //std::vector<Triangle*> get_triangles();
    std::vector<Triangle*> get_light_emitting_triangles();
    std::vector<Sphere*> get_spheres();
private:
    //PhotonMapper photonMapper;
    std::vector<Object3d*> objects;
    std::vector<Triangle*> light_emitting_triangles; // Calucated before render
    std::vector<Sphere*> spheres;

    void CreateDefaultScene();
};

#endif //TNCG15_PROJ_SCENE_H
