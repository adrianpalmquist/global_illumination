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
    vec3 direction;
    vec3 origin = camera.get_camera_position();

    // Render pixel multiple times for path tracing
    for (int pathtracing_sample = 0; pathtracing_sample < PATH_TRACING_MAX_SAMPLES; pathtracing_sample++) {

        // Loop through each pixel in the image
        for (int y = 0; y < Camera::CAMERA_HEIGHT; y++) {
            for (int x = 0; x < Camera::CAMERA_WIDTH; x++) {
                ColorDbl pixel_color = ColorDbl(0.0, 0.0, 0.0);
                // Use sampled directions for anti aliasing
                sampled_directions = camera.get_camera_direction(x, y);
                for (std::vector<vec3>::iterator it = sampled_directions.begin(); it != sampled_directions.end(); ++it) {
                    direction = *it;
                    Ray *parent_ray = new Ray(nullptr, new vec3(origin), direction, 0);
                    ray_tracer.StartRayTracing(parent_ray);
                    pixel_color += ColorFromRayTree(parent_ray) / ((float) ANTI_ALIASING_SAMPLES);
                    delete parent_ray;
                }
                //progessIndicator(((double)(x+y*Camera::CAMERA_HEIGHT))/( (double)(Camera::CAMERA_HEIGHT * Camera::CAMERA_WIDTH)));
                //pixel_color = pixel_color / ((float) PATH_TRACING_MAX_SAMPLES);
                camera.set_pixel_clr(x, y,  pixel_color + camera.get_pixel_clr(x,y));
            }
        }

        std::cout << "Path tracing iteration number: " <<  pathtracing_sample + 1 << std::endl;
        CreateImage(pathtracing_sample + 1);

    }
}

ColorDbl Renderer::ColorFromRayTree(Ray *parentRay) {
    Ray* ray = Traverse(parentRay);
    //assert(ray != nullptr);
    //std::cout << "Color: " << ray->get_ray_color().get_rgb().r<< ray->get_ray_color().get_rgb().g << ray->get_ray_color().get_rgb().b << std::endl;
    return ray->get_ray_color();
}

double clamp(double input, double min, double max) {
    if (input < min)
        return min;

    if (input > max)
        return max;

    return input;
}






Ray* Renderer::Traverse(Ray* ray) {
    if (ray != nullptr){
        Traverse(ray->get_reflected_ray());
        Traverse(ray->get_transmitted_ray());
        if (ray->get_parent_ray() != nullptr) {
            ray->get_parent_ray()->add_ray_color(ray->get_ray_color() * ray->get_radiance_distribution());
        }
    }
    return ray;
}




void Renderer::CreateImage(int pathtracing_iteration) {
    double max = 0.0;
//    double temp;
//    for (int x = 0; x < Camera::CAMERA_WIDTH; x++) {
//        for (int y = 0; y < Camera::CAMERA_HEIGHT; y++) {
//            camera.set_pixel_clr(x,y, camera.get_pixel_clr(x,y) / ((double) pathtracing_iteration));
//            temp = camera.get_pixel_clr(x,y).get_clr_magnitude();
//            if (temp > max) {
//                max = temp;
//            }
//        }
//    }

    FILE *fp = fopen("render2.ppm", "wb"); /* b - binary mode */
    (void) fprintf(fp, "P6\n%d %d\n255\n", Camera::CAMERA_WIDTH, Camera::CAMERA_HEIGHT);
    static unsigned char color[3];
    double temp_r, temp_g, temp_b;
    for (int y = 0; y < Camera::CAMERA_WIDTH; y++) {
        for (int x = 0; x < Camera::CAMERA_HEIGHT; x++) {
            ColorDbl pixel_color = camera.get_pixel_clr(x,y) / ((double) pathtracing_iteration);

            temp_r = sqrt(clamp(pixel_color.get_rgb().r, 0.0, 1.0));
            temp_g = sqrt(clamp(pixel_color.get_rgb().g, 0.0, 1.0));
            temp_b = sqrt(clamp(pixel_color.get_rgb().b, 0.0, 1.0));
            max = sqrt(1.0); // in case max != 1

            color[0] = (unsigned char) (temp_r * (255.99 / max));
            color[1] = (unsigned char) (temp_g * (255.99 / max));
            color[2] = (unsigned char) (temp_b * (255.99 / max));

            (void) fwrite(color, 1, 3, fp);
        }
    }
    (void) fclose(fp);
}

void Renderer::progessIndicator(double progress) {
    int barWidth = 70;
    int pos = (int) (barWidth * progress);
    if (progress_indicator != pos) {
        std::cout.flush();
        std::cout << "[";
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << " %\r";
        std::cout << std::endl;
        progress_indicator = pos;
    }
}
