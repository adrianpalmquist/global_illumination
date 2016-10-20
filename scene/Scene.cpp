//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include "Scene.h"
#include "../Ray.h"
#include "../material/BaseMaterial.h"
#include "../material/DiffuseMaterial.h"
#include "../material/SpecularMaterial.h"
#include "../material/EmissionMaterial.h"
#include "../material/TransmissionMaterial.h"


Scene::Scene() {
    CreateDefaultScene();
}

void Scene::CreateDefaultScene() {
    // Materials
    BaseMaterial* diffuse_red_material = new DiffuseMaterial(ColorRGB(1,0,0));
    BaseMaterial* diffuse_green_material = new DiffuseMaterial(ColorRGB(0,1,0));
    BaseMaterial* diffuse_blue_material = new DiffuseMaterial(ColorRGB(0,0,1));
    BaseMaterial* diffuse_yellow_material = new DiffuseMaterial(ColorRGB(1,1,0));
    BaseMaterial* diffuse_white_material = new DiffuseMaterial(ColorRGB(1,1,1));
    BaseMaterial* specular_white_material = new SpecularMaterial(ColorRGB(1,1,1));
    BaseMaterial* transmission_material = new TransmissionMaterial(ColorRGB(1,1,1));
    BaseMaterial* emission_material = new EmissionMaterial(ColorRGB(1,1,1), 1000);

    // Room objects
    Object3d floor = Object3d::loadObj("../models/floor.obj", diffuse_white_material);
    Object3d roof = Object3d::loadObj("../models/roof.obj", diffuse_white_material);
    Object3d left_wall = Object3d::loadObj("../models/left_wall.obj", diffuse_green_material);
    Object3d right_wall = Object3d::loadObj("../models/right_wall.obj", diffuse_red_material);

    objects.push_back(floor);
    objects.push_back(roof);
    objects.push_back(left_wall);
    objects.push_back(right_wall);

    // Light object
    Object3d light_plane = Object3d::loadObj("../models/light_plane.obj", emission_material);
    objects.push_back(light_plane);

    // Teapot object
    //Object3d teapot = Object3d::loadObj("models/teapot.obj");
    //objects.push_back(teapot);

    // Spheres
    //spheres.push_back(new Sphere(vec3(5,-4,-2), 2.0f, diffuse_white_material));
    //spheres.push_back(new Sphere(vec3(10,1.5,-2), 2.5f, diffuse_white_material));
}

Scene::~Scene() {}

std::vector<Sphere *> Scene::get_spheres() {
    return spheres;
}

bool Scene::RayIntersection(Ray ray, vec3 &collision_pos, vec3 &collision_normal, BaseMaterial *&collision_material) {
    bool collision = false;

    // Calculate triangle collision
    for (int o = 0; o < objects.size(); o++) {
        Object3d object = objects.at(o);
        // Check for object bounding box collision
        if (object.BoundingBoxCollision(ray)) {
            for (int tri = 0; tri < object.get_triangles().size(); tri++) {
                Triangle* triangle = object.get_triangles().at(tri);
                if (triangle->RayIntersection(ray.get_start_point(), ray.get_direction(), collision_pos)) {
                    collision = true;
                    collision_normal = triangle->get_normal();
                    collision_material = object.get_material();
                    break;
                }
            }
        }
    }

    // Calculate sphere intersection
    for (std::vector<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); ++it) {
        Sphere * sphere = *it;
        if (sphere->RayIntersection(ray.get_start_point(), ray.get_direction(), collision_pos, collision_normal)) {
            collision_material = sphere->get_material();
            collision = true;
            break;
        }
    }

    return collision;

}

bool Scene::RayIntersection(Ray ray, vec3 &collision_pos) {
    bool collision = false;

    // Calculate triangle collision
    for (int o = 0; o < objects.size(); o++) {
        Object3d object = objects.at(o);

        // Check for object bounding box collision
        if (object.BoundingBoxCollision(ray)) {
            for (int tri = 0; tri < object.get_triangles().size(); tri++) {
                Triangle* triangle = object.get_triangles().at(tri);
                if (triangle->RayIntersection(ray.get_start_point(), ray.get_direction(), collision_pos)) {
                    collision = true;
                    break;
                }
            }
        }
    }

    // Calculate sphere intersection
    for (std::vector<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); ++it) {
        Sphere * sphere = *it;
        vec3 collision_normal;
        if (sphere->RayIntersection(ray.get_start_point(), ray.get_direction(), collision_pos, collision_normal)) {
            collision = true;
            break;
        }
    }

    return collision;

}

void Scene::PrepareForRender() {
    // Pre calculate which triangles emit light
    for (std::vector<Object3d>::iterator it = objects.begin(); it != objects.end(); ++it) {
        Object3d object = *it;
        for (int tri = 0; tri < object.get_triangles().size(); tri++) {
            Triangle *triangle = object.get_triangles().at(tri);
            if (object.get_material()->get_flux() != 0) {
                light_emitting_triangles.push_back(triangle);
            }
        }
    }
}

std::vector<Triangle *> Scene::get_light_emitting_triangles() {
    return light_emitting_triangles;
}
