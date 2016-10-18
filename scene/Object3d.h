//
// Created by Filip K on 18/10/16.
//

#ifndef TNCG15_PROJ_OBJECT3D_H
#define TNCG15_PROJ_OBJECT3D_H


#include <string>
#include "Triangle.h"
#include "BoundingBox.h"

class Object3d {
public:
    Object3d();
    Object3d(std::vector<Triangle*> _triangles);

    bool BoundingBoxCollision(Ray ray);

    void set_triangles(std::vector<Triangle*> _triangles);
    std::vector<Triangle*> get_triangles();
    void add_triangle(Triangle* _triangle);
    void set_smooth_shading(bool smooth_shade);
    void set_position(vec3 _position);

    static Object3d loadObj(std::string _filename);
private:
    void CalculateBoundingBox();

    vec3 position;
    vec3 scale;
    vec3 rotation;

    std::vector<Triangle*> triangles;
    BoundingBox bounding_box;
    bool smooth_shading = false;
};


#endif //TNCG15_PROJ_OBJECT3D_H
