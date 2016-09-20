//
// Created by Filip Kantedal on 06/09/16.
//
#pragma once

#include <vector>
#include "Sphere.h"
#include "Triangle.h"

using namespace std;

class Scene {
public:
    static const int TRIANGLE_COUNT = 20;
    static const int SPHERE_COUNT = 2;

    Scene();

    ~Scene();

    Triangle * get_triangle_at(int index);
    Sphere * get_sphere_at(int index);
private:
    Triangle * triangles[TRIANGLE_COUNT];
    Sphere * spheres[SPHERE_COUNT];

    void CreateDefaultScene();
};

Scene::Scene() {
    CreateDefaultScene();
}

Triangle* Scene::get_triangle_at(int index) {
    return triangles[index];
}

void Scene::CreateDefaultScene() {
    // Vertices
    vec3 *v1 = new vec3(-3, 0, -5);
    vec3 *v2 = new vec3(0, 6, -5);
    vec3 *v3 = new vec3(0, -6, -5);
    vec3 *v4 = new vec3(10, 6, -5);
    vec3 *v5 = new vec3(13, 0, -5);
    vec3 *v6 = new vec3(10, -6, -5);
    vec3 *v7 = new vec3(-3, 0, 5);
    vec3 *v8 = new vec3(0, 6, 5);
    vec3 *v9 = new vec3(0, -6, 5);
    vec3 *v10 = new vec3(10, 6, 5);
    vec3 *v11 = new vec3(13, 0, 5);
    vec3 *v12 = new vec3(10, -6, 5);

    // Colors
    ColorDbl* bottom_clr = new ColorDbl(1,0,1);
    ColorDbl* top_clr = new ColorDbl(0,1,1);
    ColorDbl* right_wall_clr1 = new ColorDbl(1,0,0);
    ColorDbl* right_wall_clr2 = new ColorDbl(1,1,0.5);
    ColorDbl* right_wall_clr3 = new ColorDbl(1,0,0.5);
    ColorDbl* left_wall_clr1 = new ColorDbl(0,0,1);
    ColorDbl* left_wall_clr2 = new ColorDbl(0.5,1,0);
    ColorDbl* left_wall_clr3 = new ColorDbl(1,0.2,0);

    // Generate triangles (See image in assets folder)

    // Bottom triangles
    triangles[0] = new Triangle(v1, v3, v2, bottom_clr);
    triangles[1] = new Triangle(v2, v3, v4, bottom_clr);
    triangles[2] = new Triangle(v4, v3, v6, bottom_clr);
    triangles[3] = new Triangle(v4, v6, v5, bottom_clr);

    // Top triangles
    triangles[4] = new Triangle(v7, v8, v9, top_clr);
    triangles[5] = new Triangle(v8, v10, v9, top_clr);
    triangles[6] = new Triangle(v10, v12, v9, top_clr);
    triangles[7] = new Triangle(v10, v11, v12, top_clr);

    // Left wall triangles
    triangles[8] = new Triangle(v7, v1, v8, left_wall_clr1);
    triangles[9] = new Triangle(v8, v1, v2, left_wall_clr1);
    triangles[10] = new Triangle(v8, v2, v10, left_wall_clr2);
    triangles[11] = new Triangle(v2, v4, v10, left_wall_clr2);
    triangles[12] = new Triangle(v10, v4, v11, left_wall_clr3);
    triangles[13] = new Triangle(v11, v4, v5, left_wall_clr3);

    // Right wall triangles
    triangles[14] = new Triangle(v7, v9, v1, right_wall_clr1);
    triangles[15] = new Triangle(v9, v3, v1, right_wall_clr1);
    triangles[16] = new Triangle(v9, v12, v3, right_wall_clr2);
    triangles[17] = new Triangle(v12, v6, v3, right_wall_clr2);
    triangles[18] = new Triangle(v12, v11, v6, right_wall_clr3);
    triangles[19] = new Triangle(v11, v5, v6, right_wall_clr3);

    //Spheres
    spheres[0] = new Sphere(vec3(5,-3,-4.5), 2, new ColorDbl(1,0,0));
    spheres[1] = new Sphere(vec3(10,3,-4.5), 2, new ColorDbl(0,1,0));
}

Sphere *Scene::get_sphere_at(int index) {
    return spheres[index];
}

Scene::~Scene() {
}
