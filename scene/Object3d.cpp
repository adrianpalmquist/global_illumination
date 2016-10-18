//
// Created by Filip K on 18/10/16.
//

#include <iostream>
#include "Object3d.h"

Object3d::Object3d() {}

Object3d::Object3d(std::vector<Triangle *> _triangles) {
    position = vec3(0,0,0);
    triangles = _triangles;
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

Object3d Object3d::loadObj(std::string _filename) {
    return Object3d();
}

bool Object3d::BoundingBoxCollision(Ray ray) {
    return bounding_box.RayIntersection(ray);
}

