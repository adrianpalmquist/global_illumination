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
    Scene *scene;
    Camera camera;

    ColorDbl ColorFromRayTree(Ray *parentRay);
};

Renderer::Renderer() {
    scene = new Scene();
    camera = Camera();
}

void Renderer::Render() {
    ColorDbl black = ColorDbl(vec3(0.0,0.0,0.0));
    vec3 origin = camera.get_camera_position();
    for (int y = 0; y < Camera::CAMERA_HEIGHT; y++) {
        for (int x = 0; x < Camera::CAMERA_WIDTH; x++) {
            vec3 direction = camera.get_camera_direction(x,y);
            vec3 *start_point = new vec3(origin);
            Ray *parent_ray = new Ray(start_point, direction, 0);
            parent_ray->TraceRay(scene);
            ColorDbl clr = ColorFromRayTree(parent_ray);
            delete parent_ray;

            camera.set_pixel_clr(x,y,clr);
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

