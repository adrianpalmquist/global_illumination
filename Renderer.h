//
// Created by Adrian Palmquist on 2016-09-16.
//

#pragma once

#include "Scene.h"
#include "Camera.h"

class Renderer {
public:
    Renderer();
    void Render();
    void CreateImage();
    //ColorDbl TraceRay(vec3 origin, vec3 direction, bool material);

private:
    Scene *scene;
    Camera camera;

    ColorDbl ColorFromRayTree(Ray *parentRay);

//    int

    int iterations = 1;
    float n1 = 1.000293; // air
    float n2 = 1.52; // glass
    const int SS = 4;

};

Renderer::Renderer() {
    scene = new Scene();
    camera = Camera();
}

void Renderer::Render() {
    vector<vec3> sampled_directions;
    ColorDbl clr;
    int counter;
    vec3 direction;
    ColorDbl black = ColorDbl(vec3(0.0,0.0,0.0));
    vec3 origin = camera.get_camera_position();
    for (int y = 0; y < Camera::CAMERA_HEIGHT; y++) {
        for (int x = 0; x < Camera::CAMERA_WIDTH; x++) {
            sampled_directions = camera.get_camera_direction(x,y);
            counter = 0;
            for (std::vector<vec3>::iterator it = sampled_directions.begin(); it != sampled_directions.end(); ++it) {
                direction = *it;
                vec3 *start_point = new vec3(origin);
                Ray *parent_ray = new Ray(start_point, direction, 0);
                parent_ray->TraceRay(scene);
                clr += ColorFromRayTree(parent_ray);
                delete parent_ray;
                counter++;
            }
            clr = clr/SS;
            camera.set_pixel_clr(x, y, clr);
        }
    }
}

ColorDbl Renderer::ColorFromRayTree(Ray *parentRay) {
    Ray* currentRay = parentRay;
    while (currentRay->get_reflected_ray() != 0) {
        currentRay = currentRay->get_reflected_ray();
    }
    return currentRay->get_color();
}


void Renderer::CreateImage() {
        double max = 0.0;
        double temp;
        for (int x = 0; x < Camera::CAMERA_WIDTH; x++) {
            for (int y = 0; y < Camera::CAMERA_HEIGHT; y++) {
                temp = camera.get_pixel_clr(x,y).get_clr_magnitude();
                if (temp > max) {
                    max = temp;
                }
            }
        }

        FILE *fp = fopen("first.ppm", "wb"); /* b - binary mode */
        (void) fprintf(fp, "P6\n%d %d\n255\n", Camera::CAMERA_WIDTH, Camera::CAMERA_HEIGHT);
        static unsigned char color[3];
        double temp_r, temp_g, temp_b;
        for (int x = 0; x < Camera::CAMERA_WIDTH; x++) {
            for (int y = 0; y < Camera::CAMERA_HEIGHT; y++) {
                temp_r = sqrt(camera.get_pixel_clr(x,y).get_rgb().r);
                temp_g = sqrt(camera.get_pixel_clr(x,y).get_rgb().g);
                temp_b = sqrt(camera.get_pixel_clr(x,y).get_rgb().b);
                max = sqrt(max); // in case max != 1
                color[0] = (unsigned char) (temp_r * (255.99 / max));
                color[1] = (unsigned char) (temp_g * (255.99 / max));
                color[2] = (unsigned char) (temp_b * (255.99 / max));
                (void) fwrite(color, 1, 3, fp);
            }
        }
        (void) fclose(fp);
}

//ColorDbl Renderer::TraceRay(vec3 origin, vec3 direction, bool material) {
//    vec3 intersection_point;
//    ColorDbl clr;
//    //Calculate triangle collision
//    for (int tri_idx = 0; tri_idx < Scene::TRIANGLE_COUNT; tri_idx++) {
//        Triangle * triangle = scene.get_triangle_at(tri_idx);
//        if(triangle->rayIntersection(origin, direction, intersection_point)) {
//            //std::cout << triangle->get_normal().x << " " << triangle->get_normal().y << " " << triangle->get_normal().z << std::endl;
//            dvec3 new_clr = (double) (1 - dot(triangle->get_normal(), direction)) * triangle->get_clr()->get_rgb();
//            clr = ColorDbl(new_clr);
//            break;
//        }
//    }
//
//    //Calculate sphere intersection
//    vec3 collision_normal;
//    for(int sphere_idx = 0; sphere_idx < Scene::SPHERE_COUNT; sphere_idx++) {
//        Sphere * sphere = scene.get_sphere_at(sphere_idx);
//        if(sphere->RayIntersection(origin, direction, intersection_point, collision_normal)) {
//            if (material == 0) {
//                n1 = 1.000293;
//                n2 = 1.52;
//            }
//            else {
//                n2 = 1.000293;
//                n1 = 1.52;
//            }
//            material = !material;
//            dvec3 new_clr = (double) (1 - dot(collision_normal, direction)) * sphere->get_clr()->get_rgb();
//            clr = ColorDbl(new_clr);
//            vec3 reflected = normalize(direction - 2 * dot(direction,collision_normal) * collision_normal);
//            vec3 transmitted = normalize((n1/n2) * reflected + collision_normal * (float) ( (-(n1/n2) * dot(collision_normal,reflected) - sqrt(1 - pow((n1/n2),2) * (1 - (pow(dot(collision_normal,reflected),2)))))));
//            double angle = acos(dot(-direction,collision_normal));
//            std::cout << "angle: " << angle << std::endl;
//            if (material == 1 && angle < 0.73) {
//                double r_s = pow((n1 * cos(angle) - n2 * sqrt(1 - pow((n1 / n2) * sin(angle), 2))) /
//                                 (n1 * cos(angle) + n2 * sqrt(1 - pow((n1 / n2) * sin(angle), 2))), 2);
//                double r_p = pow((n1 * sqrt(1 - pow((n1 / n2) * sin(angle), 2)) - n2 * cos(angle)) /
//                                 (n1 * sqrt(1 - pow((n1 / n2) * sin(angle), 2)) + n2 * cos(angle)), 2);
//                double r = (r_s + r_p) / 2;
//                return TraceRay(origin, transmitted, material);
//            } else {
//                return TraceRay(origin,reflected,material);
//            }
//
//            //break;
//        }
//    }
//
//    return clr;
//}
