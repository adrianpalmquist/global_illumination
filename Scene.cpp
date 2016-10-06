//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include "Scene.h"
#include "Ray.h"
#include "material/BaseMaterial.h"
#include "material/DiffuseMaterial.h"
#include "material/SpecularMaterial.h"


Scene::Scene() {
    CreateDefaultScene();
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

    // Materials
    BaseMaterial* diffuse_red_material = new DiffuseMaterial(ColorDbl(1,0,0));
    BaseMaterial* diffuse_green_material = new DiffuseMaterial(ColorDbl(0,1,0));
    BaseMaterial* diffuse_blue_material = new DiffuseMaterial(ColorDbl(0,0,1));
    BaseMaterial* diffuse_yellow_material = new DiffuseMaterial(ColorDbl(1,1,0));
    BaseMaterial* diffuse_white_material = new DiffuseMaterial(ColorDbl(1,1,1));
    BaseMaterial* specular_white_material = new SpecularMaterial(ColorDbl(1,1,1));

    // Light source
    BaseMaterial* light_material = new DiffuseMaterial(ColorDbl(1,1,1));
    light_material->enable_light_emission();
    light_material->set_light_color(ColorDbl(1,1,0.6));
    light_material->set_flux(3.0f);
    vec3 *v13 = new vec3(10,-2,4);
    vec3 *v14 = new vec3(10,2,4);
    vec3 *v15 = new vec3(12,-2,2);
    vec3 *v16 = new vec3(12,2,2);
    triangles.push_back(new Triangle(v13, v14, v15, light_material));
    triangles.push_back(new Triangle(v14, v16, v15, light_material));

    // Generate triangles (See image in assets folder)

    // Bottom triangles
    triangles.push_back(new Triangle(v1, v3, v2, diffuse_white_material));
    triangles.push_back(new Triangle(v2, v3, v4, diffuse_white_material));
    triangles.push_back(new Triangle(v4, v3, v6, diffuse_white_material));
    triangles.push_back(new Triangle(v4, v6, v5, diffuse_white_material));

    // Top triangles
    triangles.push_back(new Triangle(v7, v8, v9, diffuse_white_material));
    triangles.push_back(new Triangle(v8, v10, v9, diffuse_white_material));
    triangles.push_back(new Triangle(v10, v12, v9, diffuse_white_material));
    triangles.push_back(new Triangle(v10, v11, v12, diffuse_white_material));

    // Left wall triangles
    triangles.push_back(new Triangle(v7, v1, v8, diffuse_yellow_material));
    triangles.push_back(new Triangle(v8, v1, v2, diffuse_yellow_material));
    triangles.push_back(new Triangle(v8, v2, v10, diffuse_yellow_material));
    triangles.push_back(new Triangle(v2, v4, v10, diffuse_blue_material));
    triangles.push_back(new Triangle(v10, v4, v11, diffuse_blue_material));
    triangles.push_back(new Triangle(v11, v4, v5, diffuse_blue_material));

    // Right wall triangles
    triangles.push_back(new Triangle(v7, v9, v1, diffuse_red_material));
    triangles.push_back(new Triangle(v9, v3, v1, diffuse_red_material));
    triangles.push_back(new Triangle(v9, v12, v3, diffuse_red_material));
    triangles.push_back(new Triangle(v12, v6, v3, diffuse_green_material));
    triangles.push_back(new Triangle(v12, v11, v6, diffuse_green_material));
    triangles.push_back(new Triangle(v11, v5, v6, diffuse_green_material));

    // Spheres
    spheres.push_back(new Sphere(vec3(5,-4,-2), 2, specular_white_material));
    spheres.push_back(new Sphere(vec3(10,2,-2), 2.5, specular_white_material));
}

Scene::~Scene() {}

std::vector<Triangle *> Scene::get_triangles() {
    return triangles;
}

std::vector<Sphere *> Scene::get_spheres() {
    return spheres;
}

bool Scene::RayIntersection(vec3 start_point, vec3 direction, vec3 &collision_pos, vec3 &collision_normal, BaseMaterial *&collision_material) {
    bool collision = false;

    //Calculate triangle collision
    for (std::vector<Triangle*>::iterator it = triangles.begin(); it != triangles.end(); ++it) {
        Triangle *triangle = *it;
        if (triangle->RayIntersection(start_point, direction, collision_pos)) {
            collision = true;
            collision_normal = triangle->get_normal();
            collision_material = triangle->get_material();
            break;
        }
    }

    //Calculate sphere intersection
    for (std::vector<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); ++it) {
        Sphere * sphere = *it;
        if (sphere->RayIntersection(start_point, direction, collision_pos, collision_normal)) {
            collision_material = sphere->get_material();
            collision = true;
            break;
        }
    }

    if (collision) return true;

    return false;
}

bool Scene::RayIntersection(vec3 start_point, vec3 direction, vec3 &collision_pos) {
    bool collision = false;

    //Calculate triangle collision
    for (std::vector<Triangle*>::iterator it = triangles.begin(); it != triangles.end(); ++it) {
        Triangle *triangle = *it;
        if (triangle->RayIntersection(start_point, direction, collision_pos)) {
            collision = true;
            break;
        }
    }

    //Calculate sphere intersection
    for (std::vector<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); ++it) {
        Sphere * sphere = *it;
        vec3 collision_normal;
        if (sphere->RayIntersection(start_point, direction, collision_pos, collision_normal)) {
            collision = true;
            break;
        }
    }

    if (collision) return true;

    return false;
}

void Scene::PrepareForRender() {
    // Pre calculate which triangles emit light
    for (std::vector<Triangle*>::iterator it = triangles.begin(); it != triangles.end(); ++it) {
        Triangle *triangle = *it;
        if (triangle->get_material()->is_emitting_light()) {
            light_emitting_triangles.push_back(triangle);
        }
    }
}

std::vector<Triangle *> Scene::get_light_emitting_triangles() {
    return light_emitting_triangles;
}
