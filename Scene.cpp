//
// Created by Filip Kantedal on 23/09/16.
//

#include "Scene.h"
#include "Ray.h"


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
    triangles.push_back(new Triangle(v1, v3, v2, bottom_clr));
    triangles.push_back(new Triangle(v2, v3, v4, bottom_clr));
    triangles.push_back(new Triangle(v4, v3, v6, bottom_clr));
    triangles.push_back(new Triangle(v4, v6, v5, bottom_clr));

    // Top triangles
    triangles.push_back(new Triangle(v7, v8, v9, top_clr));
    triangles.push_back(new Triangle(v8, v10, v9, top_clr));
    triangles.push_back(new Triangle(v10, v12, v9, top_clr));
    triangles.push_back(new Triangle(v10, v11, v12, top_clr));

    // Left wall triangles
    triangles.push_back(new Triangle(v7, v1, v8, left_wall_clr1));
    triangles.push_back(new Triangle(v8, v1, v2, left_wall_clr1));
    triangles.push_back(new Triangle(v8, v2, v10, left_wall_clr2));
    triangles.push_back(new Triangle(v2, v4, v10, left_wall_clr2));
    triangles.push_back(new Triangle(v10, v4, v11, left_wall_clr3));
    triangles.push_back(new Triangle(v11, v4, v5, left_wall_clr3));

    // Right wall triangles
    triangles.push_back(new Triangle(v7, v9, v1, right_wall_clr1));
    triangles.push_back(new Triangle(v9, v3, v1, right_wall_clr1));
    triangles.push_back(new Triangle(v9, v12, v3, right_wall_clr2));
    triangles.push_back(new Triangle(v12, v6, v3, right_wall_clr2));
    triangles.push_back(new Triangle(v12, v11, v6, right_wall_clr3));
    triangles.push_back(new Triangle(v11, v5, v6, right_wall_clr3));

    //Spheres
    spheres.push_back(new Sphere(vec3(5,-3,-4.5), 2, new ColorDbl(1,0,0)));
    spheres.push_back(new Sphere(vec3(10,3,-4.5), 2, new ColorDbl(0,1,0)));
}

Scene::~Scene() {
}

std::vector<Triangle *> Scene::get_triangles() {
    return triangles;
}

std::vector<Sphere *> Scene::get_spheres() {
    return spheres;
}

bool Scene::RayIntersection(Ray *ray, vec3 &collision_pos, vec3 &collision_normal, bool &reflect) {
    bool collision = false;
    reflect = false;

    //Calculate triangle collision
    for (std::vector<Triangle*>::iterator it = triangles.begin(); it != triangles.end(); ++it) {
        Triangle *triangle = *it;
        if (triangle->rayIntersection(*ray->get_start_point(), ray->get_direction(), collision_pos)) {
            ray->set_ray_color(ColorDbl(/*(double) (1 - dot(triangle->get_normal(), ray->get_direction())) * */ triangle->get_clr()->get_rgb()));
            collision_normal = triangle->get_normal();
            break;
        }
    }

    //Calculate sphere intersection
    for (std::vector<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); ++it) {
        Sphere * sphere = *it;
        if (sphere->RayIntersection(*ray->get_start_point(), ray->get_direction(), collision_pos, collision_normal)) {
            ray->set_ray_color(ColorDbl((double) (1 - dot(collision_normal, ray->get_direction())) * sphere->get_clr()->get_rgb()));
            collision = true;
            reflect = true;
            break;
        }
    }

    if (collision)
        return true;
    return false;
}
