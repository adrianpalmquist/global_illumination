//
// Created by Filip Kantedal on 06/09/16.
//
#pragma once

#include "ColorDbl.h"
#include "Scene.h"

class Ray {
public:
    static const int MAX_ITERATIONS = 5;

    Ray(vec3 *_start_point, vec3 _direction, int _ray_iteration);
    ~Ray();

    void TraceRay(Scene* scene);
    void CalculateClr(Scene* scene);

    int get_ray_iterations();
    Ray* get_reflected_ray();
    Ray* get_transmitted_ray();
    Ray* set_radiance_distribution(double _radiance_distribution);
    ColorDbl get_color();
private:
    vec3 * start_point;
    vec3 * end_point;
    vec3 direction;
    ColorDbl clr;
    int ray_iteration;
    double radiance_distribution = 1.0;
    float n1 = 1.000293; // air
    float n2 = 1.52; // glass

    Ray* reflected_ray = 0;
    Ray* transmitted_ray = 0;

    bool ReflectRay(vec3 collision_normal, vec3 in_direction, vec3 &out_direction);
    bool TransmitRay(vec3 collision_normal, vec3 in_direction, bool going_in, vec3 &out_direction, vec3 reflected_direction, double &radiance_distribution);
};

Ray::Ray(vec3 *_start_point, vec3 _direction, int _ray_iteration) : start_point(_start_point), direction(_direction) {
    ray_iteration = _ray_iteration + 1;
}

Ray::~Ray() {

}

bool Ray::ReflectRay(vec3 collision_normal, vec3 in_direction, vec3 &out_direction) {
    if (!(collision_normal.x == 0 && collision_normal.y == 0 && collision_normal.z == 0)) {
        out_direction = normalize(in_direction - 2 * dot(in_direction, collision_normal) * collision_normal);
        return true;
    }

    return false;
}

bool Ray::TransmitRay(vec3 collision_normal, vec3 in_direction, bool going_in, vec3 &out_direction, vec3 reflected_direction, double &radiance_distribution) {
    if (!(collision_normal.x == 0 && collision_normal.y == 0 && collision_normal.z == 0)) {
        if (going_in) {
            n1 = 1.000293;
            n2 = 1.52;
        } else {
            n1 = 1.52;
            n2 = 1.000293;
        }
        out_direction = normalize((n1 / n2) * reflected_direction + collision_normal * (float) ((
                -(n1 / n2) * dot(collision_normal, reflected_direction) -
                sqrt(1 - pow((n1 / n2), 2) * (1 - (pow(dot(collision_normal, reflected_direction), 2)))))));
        double angle = acos(dot(-in_direction, collision_normal));
        //std::cout << "angle: " << angle << std::endl;
        if (!going_in && angle < 0.73) {
            double r_s = pow((n1 * cos(angle) - n2 * sqrt(1 - pow((n1 / n2) * sin(angle), 2))) /
                             (n1 * cos(angle) + n2 * sqrt(1 - pow((n1 / n2) * sin(angle), 2))), 2);
            double r_p = pow((n1 * sqrt(1 - pow((n1 / n2) * sin(angle), 2)) - n2 * cos(angle)) /
                             (n1 * sqrt(1 - pow((n1 / n2) * sin(angle), 2)) + n2 * cos(angle)), 2);
            radiance_distribution = (r_s + r_p) / 2;
        }
        return true;
    }

    return false;
}


void Ray::TraceRay(Scene *scene) {
    vec3 collision_normal(0.0);
    vec3 reflected_direction;
    vec3 transmitted_direction;
    vec3 collision_point;
    double radiance_distribution;
    bool reflect = false;
    bool transmit = false;

    //Calculate triangle collision
    std::vector<Triangle*> triangles = scene->get_triangles();
    for (std::vector<Triangle*>::iterator it = triangles.begin(); it != triangles.end(); ++it) {

        Triangle *triangle = *it;
        if (triangle->rayIntersection(*start_point, direction, collision_point)) {
            clr = ColorDbl((double) (1 - dot(triangle->get_normal(), direction)) * triangle->get_clr()->get_rgb());
            break;
        }
    }

    //Calculate sphere intersection
    std::vector<Sphere*> spheres = scene->get_spheres();
    for (std::vector<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); ++it) {
        Sphere * sphere = *it;
        if (sphere->RayIntersection(*start_point, direction, collision_point, collision_normal)) {
            clr =  ColorDbl((double) (1 - dot(collision_normal, direction)) * sphere->get_clr()->get_rgb());
            break;
        }
    }

    end_point = new vec3(collision_point.x, collision_point.y, collision_point.z);

    // Continue building ray tree ray_iteration is less than max iterations
    if (ray_iteration <= MAX_ITERATIONS) {
        // Check if rays should be reflected/transmitted and create child rays
        if (ReflectRay(collision_normal, direction, reflected_direction)) {
            reflect = true;
            reflected_ray = new Ray(end_point, reflected_direction, ray_iteration);
        }

        if (TransmitRay(collision_normal, direction, true, transmitted_direction, reflected_direction, radiance_distribution)) {
            transmitted_ray = new Ray(end_point, transmitted_direction, ray_iteration);
            if (reflect) {
                reflected_ray->set_radiance_distribution(1-radiance_distribution);
            }
            transmitted_ray->set_radiance_distribution(radiance_distribution);
            transmitted_ray->TraceRay(scene);
        }
        if (reflect) {
            reflected_ray->TraceRay(scene);
        }
    }
}

int Ray::get_ray_iterations() {
    return ray_iteration;
}

ColorDbl Ray::get_color() {
    return clr;
}

Ray *Ray::get_reflected_ray() {
    return reflected_ray;
}

Ray *Ray::get_transmitted_ray() {
    return transmitted_ray;
}

Ray *Ray::set_radiance_distribution(double _radiance_distribution) {
    radiance_distribution = _radiance_distribution;
}

void Ray::CalculateClr(Scene *scene) {

}
