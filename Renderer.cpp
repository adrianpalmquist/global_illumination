//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include <algorithm>
#include "Renderer.h"

Renderer::Renderer() {
    scene = new Scene();
    camera = Camera();
    ray_tracer = RayTracer(scene);
}

void Renderer::Render() {
    scene->PrepareForRender();

    std::vector<vec3> sampled_directions;
    ColorDbl pixel_color;
    vec3 direction;
    vec3 origin = camera.get_camera_position();
    for (int y = 0; y < Camera::CAMERA_HEIGHT; y++) {
        for (int x = 0; x < Camera::CAMERA_WIDTH; x++) {

            // Render pixel multiple times for path tracing
            pixel_color = ColorDbl(0.0, 0.0, 0.0);
            for (int pathtracing_sample = 0; pathtracing_sample < PATHTRACING_SAMPLES; pathtracing_sample++) {

                // Use sampled directions for anti aliasing
                sampled_directions = camera.get_camera_direction(x,y);
                for (std::vector<vec3>::iterator it = sampled_directions.begin(); it != sampled_directions.end(); ++it) {
                    direction = *it;
                    Ray *parent_ray = new Ray(nullptr, new vec3(origin), direction, 0);
                    ray_tracer.StartRayTracing(parent_ray);
                    pixel_color += ColorFromRayTree(parent_ray) / ((float) ANTI_ALIASING_SAMPLES);
                    delete parent_ray;
                }
            }

            pixel_color = pixel_color / ((float) PATHTRACING_SAMPLES);
            camera.set_pixel_clr(x,y,pixel_color);
        }
    }
}

ColorDbl Renderer::ColorFromRayTree(Ray *parentRay) {
    Ray* currentRay = parentRay;
    while (currentRay->get_reflected_ray() != 0) {
        currentRay = currentRay->get_reflected_ray();
    }
    return currentRay->get_ray_color();
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

    FILE *fp = fopen("render.ppm", "wb"); /* b - binary mode */
    (void) fprintf(fp, "P6\n%d %d\n255\n", Camera::CAMERA_WIDTH, Camera::CAMERA_HEIGHT);
    static unsigned char color[3];
    double temp_r, temp_g, temp_b;
    for (int x = 0; x < Camera::CAMERA_WIDTH; x++) {
        for (int y = 0; y < Camera::CAMERA_HEIGHT; y++) {

            temp_r = sqrt(camera.get_pixel_clr(x,y).get_rgb().r < 0 ? 0 : camera.get_pixel_clr(x,y).get_rgb().r);
            temp_g = sqrt(camera.get_pixel_clr(x,y).get_rgb().g < 0 ? 0 : camera.get_pixel_clr(x,y).get_rgb().g);
            temp_b = sqrt(camera.get_pixel_clr(x,y).get_rgb().b < 0 ? 0 : camera.get_pixel_clr(x,y).get_rgb().b);
            max = sqrt(max); // in case max != 1

            color[0] = (unsigned char) (temp_r * (255.99 / max));
            color[1] = (unsigned char) (temp_g * (255.99 / max));
            color[2] = (unsigned char) (temp_b * (255.99 / max));

            (void) fwrite(color, 1, 3, fp);
        }
    }
    (void) fclose(fp);
}
