//
// Created by Filip K on 18/10/16.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "Object3d.h"
#include "../material/DiffuseMaterial.h"

Object3d::Object3d() {}

Object3d::Object3d(std::vector<Triangle *> _triangles, BaseMaterial* _material) {
    position = vec3(0,0,0);
    triangles = _triangles;
    material = _material;
    CalculateBoundingBox();
}

void Object3d::set_triangles(std::vector<Triangle *> _triangles) {
    position = vec3(0,0,0);
}

std::vector<Triangle *> Object3d::get_triangles() {
    return triangles;
}

void Object3d::add_triangle(Triangle *_triangle) {

}

void Object3d::set_smooth_shading(bool smooth_shade) {

}

void Object3d::set_position(vec3 _position) {
    position = _position;
}

void Object3d::CalculateBoundingBox() {
    vec3 min = vec3(0);
    vec3 max = vec3(0);

    for (int i = 0; i < triangles.size(); i++) {
        Triangle* triangle = triangles.at(i);
        std::vector<vec3*> vertices;
        vertices.push_back(triangle->get_v0());
        vertices.push_back(triangle->get_v1());
        vertices.push_back(triangle->get_v2());

        for (int v = 0; v < 2; v++) {
            vec3* vertex = vertices.at(v);
            min.x = vertex->x < min.x ? vertex->x : min.x;
            max.x = vertex->x > max.x ? vertex->x : max.x;

            min.y = vertex->y < min.y ? vertex->y : min.y;
            max.y = vertex->y > max.y ? vertex->y : max.y;

            min.z = vertex->z < min.z ? vertex->z : min.z;
            max.z = vertex->z > max.z ? vertex->z : max.z;
        }
    }

    bounding_box = BoundingBox(min, max);
}

Object3d Object3d::loadObj(std::string _filename, BaseMaterial* _material) {
    std::vector<vec3*> vertices;
    std::vector<Triangle*> triangles;

    std::string line;
    std::ifstream obj_file(_filename);
    if (obj_file.is_open())
    {
        while (getline(obj_file, line))
        {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "f") {
                std::string string_v0;
                std::string string_v1;
                std::string string_v2;
                iss >> string_v0 >> string_v1 >> string_v2;

                Triangle* tri = new Triangle(vertices.at(std::stoi(string_v0)-1), vertices.at(std::stoi(string_v1)-1), vertices.at(std::stoi(string_v2)-1));
                triangles.push_back(tri);
            }
            else if (type == "v") {
                std::string string_x;
                std::string string_y;
                std::string string_z;
                iss >> string_x >> string_y >> string_z;

                vertices.push_back(new vec3(std::stof(string_x), std::stof(string_y), std::stof(string_z)));
            }
        }
        obj_file.close();
    }

    return Object3d(triangles, _material);
}

bool Object3d::BoundingBoxCollision(Ray ray) {
    return bounding_box.RayIntersection(ray);
}

void Object3d::Translate(vec3 translation) {
}

BaseMaterial *Object3d::get_material() {
    return material;
}

