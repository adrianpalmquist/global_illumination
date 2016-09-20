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
    ColorDbl TraceRay(vec3 origin, vec3 direction);

private:
    Scene scene;
    Camera camera;
    int iterations = 1;
};

Renderer::Renderer() {
    scene = Scene();
    camera = Camera();
}

void Renderer::Render() {
    ColorDbl black = ColorDbl(vec3(0.0,0.0,0.0));
    vec3 origin = camera.get_camera_position();
    for (int y = 0; y < Camera::CAMERA_HEIGHT; y++) {
        for (int x = 0; x < Camera::CAMERA_WIDTH; x++) {
            vec3 direction = camera.get_camera_direction(x,y);
            ColorDbl clr = TraceRay(origin, direction);
            camera.set_pixel_clr(x,y,clr);
        }
    }
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
        std::cout << max << std::endl;
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

ColorDbl Renderer::TraceRay(vec3 origin, vec3 direction) {
    vec3 intersection_point;
    ColorDbl clr;
    //Calculate triangle collision
    for (int tri_idx = 0; tri_idx < Scene::TRIANGLE_COUNT; tri_idx++) {
        Triangle * triangle = scene.get_triangle_at(tri_idx);
        if(triangle->rayIntersection(origin, direction, intersection_point)) {
            //std::cout << triangle->get_normal().x << " " << triangle->get_normal().y << " " << triangle->get_normal().z << std::endl;
            dvec3 new_clr = (double) (1 - dot(triangle->get_normal(), direction)) * triangle->get_clr()->get_rgb();
            clr = ColorDbl(new_clr);
            break;
        }
    }

    //Calculate sphere intersection
    vec3 collision_normal;
    for(int sphere_idx = 0; sphere_idx < Scene::SPHERE_COUNT; sphere_idx++) {
        Sphere * sphere = scene.get_sphere_at(sphere_idx);

        if(sphere->RayIntersection(origin, direction, intersection_point, collision_normal)) {
            dvec3 new_clr = (double) (1 - dot(collision_normal, direction)) * sphere->get_clr()->get_rgb();
            clr = ColorDbl(new_clr);
            vec3 reflection = normalize(direction - 2 * dot(direction,collision_normal) * collision_normal);
            return TraceRay(origin, reflection);
            //break;
        }
    }

    return clr;
}
