//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include "Scene.h"
#include "../Ray.h"
#include "../material/BaseMaterial.h"
#include "../material/SpecularMaterial.h"
#include "../material/EmissionMaterial.h"
#include "../material/OrenNayarMaterial.h"
#include "../material/TransmissionMaterial.h"
#include "../material/DiffuseMaterial.h"


Scene::Scene() {
    CreateDefaultScene();
}

void Scene::CreateDefaultScene() {
    // Materials
    BaseMaterial* diffuse_red_material = new OrenNayarMaterial(ColorRGB(1,0,0));
    BaseMaterial* diffuse_green_material = new OrenNayarMaterial(ColorRGB(0,1,0));
    BaseMaterial* diffuse_blue_material = new OrenNayarMaterial(ColorRGB(0,0,1));
    BaseMaterial* diffuse_yellow_material = new OrenNayarMaterial(ColorRGB(1,1,0));
    BaseMaterial* diffuse_white_material = new OrenNayarMaterial(ColorRGB(1,1,1));
    BaseMaterial* specular_red_material = new SpecularMaterial(ColorRGB(0.2, 0.5, 0.6));
    BaseMaterial* specular_blue_material = new SpecularMaterial(ColorRGB(0.2, 0.2, 1.0));
    BaseMaterial* transmission_material = new TransmissionMaterial(ColorRGB(1.0, 0.3, 0.3));
    BaseMaterial* emission_material = new EmissionMaterial(ColorRGB(1,1,1), 150);

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

    Object3d box = Object3d::loadObj("../models/box.obj", diffuse_yellow_material);
    objects.push_back(box);

    Object3d pyramid = Object3d::loadObj("../models/pyramid.obj", specular_blue_material);
    objects.push_back(pyramid);


    // Teapot object
    //Object3d teapot = Object3d::loadObj("models/teapot.obj",diffuse_blue_material);
    //objects.push_back(teapot);

    // Spheres
    //spheres.push_back(new Sphere(vec3(5,-4,-2), 2.0f, diffuse_blue_material));
    //spheres.push_back(new Sphere(vec3(10,1.5,-2), 2.5f, transmission_material)); // too close to wall
    spheres.push_back(new Sphere(vec3(9,0.8, -2.5), 2.0f, transmission_material));
}

Scene::~Scene() {}

std::vector<Sphere *> Scene::get_spheres() {
    return spheres;
}

bool Scene::RayIntersection(Ray ray, vec3 &collision_pos, vec3 &collision_normal, BaseMaterial *&collision_material) {
    bool collision = false;

    // Calculate triangle collision
    for (std::vector<Object3d>::iterator obj_it = objects.begin(); obj_it != objects.end(); ++obj_it) {
        Object3d object = *obj_it;

        // Check for object bounding box collision
        if (object.BoundingBoxCollision(ray)) {
            std::vector<Triangle*> triangles = object.get_triangles();
            for (std::vector<Triangle*>::iterator tri_it = triangles.begin(); tri_it != triangles.end(); ++tri_it) {
                Triangle* triangle = *tri_it;
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
        Sphere *sphere = *it;
        if (sphere->RayIntersection(ray.get_start_point(), ray.get_direction(), collision_pos, collision_normal)) {
            collision_material = sphere->get_material();
            collision = true;
            break;
        }
    }

    return collision;

}

bool ShadowRayIntersection(Ray ray, vec3 &collision_pos) {
    bool collision = false;

//    // Calculate triangle collision
//    for (std::vector<Object3d>::iterator obj_it = objects.begin(); obj_it != objects.end(); ++obj_it) {
//        Object3d object = *obj_it;
//
//        // Check for object bounding box collision
//        if (object.BoundingBoxCollision(ray)) {
//            std::vector<Triangle*> triangles = object.get_triangles();
//            for (std::vector<Triangle*>::iterator tri_it = triangles.begin(); tri_it != triangles.end(); ++tri_it) {
//                Triangle* triangle = *tri_it;
//                if (triangle->RayIntersection(ray.get_start_point(), ray.get_direction(), collision_pos)) {
//
//                    collision = true;
//                    collision_material = object.get_material();
//                    break;
//                }
//            }
//        }
//    }
//
//    // Calculate sphere intersection
//    for (std::vector<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); ++it) {
//        Sphere *sphere = *it;
//        if (sphere->RayIntersection(ray.get_start_point(), ray.get_direction(), collision_pos, collision_normal)) {
//            collision_material = sphere->get_material();
//            collision = true;
//            break;
//        }
//    }

    return collision;
}

bool Scene::RayIntersection(Ray ray, vec3 &collision_pos) {
    bool collision = false;

//    // Calculate triangle collision
//    for (int o = 0; o < objects.size(); o++) {
//        Object3d object = objects.at(o);
//
//        // Check for object bounding box collision
//        if (object.BoundingBoxCollision(ray)) {
//            for (int tri = 0; tri < object.get_triangles().size(); tri++) {
//                Triangle* triangle = object.get_triangles().at(tri);
//                if (triangle->RayIntersection(ray.get_start_point(), ray.get_direction(), collision_pos)) {
//                    collision = true;
//                    break;
//                }
//            }
//        }
//    }

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

        if (object.get_material()->get_flux() != 0) {
            light_emitting_objects.push_back(object);
        }
    }
}

std::vector<Triangle *> Scene::get_light_emitting_triangles() {
    return light_emitting_triangles;
}

std::vector<Object3d> Scene::get_light_emitting_objects() {
    return light_emitting_objects;
}
