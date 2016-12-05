//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include <algorithm>
#include "Renderer.h"

Renderer::Renderer() {
    scene = new Scene();
    camera = Camera();
    photon_mapper = PhotonMapper(scene);
    ray_tracer = RayTracer(scene);
}

void Renderer::Render() {
    srand (time(NULL));
    const clock_t begin_time = clock();

    scene->PrepareForRender();

    // Pass 1: Photon mapping
    photon_mapper.Start();
    ray_tracer.set_photon_map(photon_mapper.get_photon_map());

    std::vector<vec3> sampled_directions;
    vec3 direction;
    vec3 origin = camera.get_camera_position();

    // Render pixel multiple times for path tracing
    for (int pathtracing_sample = 0; pathtracing_sample < Renderer::PATHTRACING_SAMPLES; pathtracing_sample++) {

        // Loop through each pixel in the image
        for (int y = 0; y < Camera::CAMERA_HEIGHT; y++) {
            for (int x = 0; x < Camera::CAMERA_WIDTH; x++) {
                ColorRGB pixel_color = ColorRGB(0.0, 0.0, 0.0);

                // Use sampled directions for anti aliasing
                sampled_directions = camera.get_camera_direction(x, y);
                for (std::vector<vec3>::iterator it = sampled_directions.begin(); it != sampled_directions.end(); ++it) {
                    direction = *it;
                    Ray parent_ray = Ray(vec3(origin), direction, 0);
                    pixel_color += ray_tracer.TraceRay(parent_ray, true) / ((float) ANTI_ALIASING_SAMPLES);
                }

                //progessIndicator(((double)(x+y*Camera::CAMERA_HEIGHT))/( (double)(Camera::CAMERA_HEIGHT * Camera::CAMERA_WIDTH)));
                //pixel_color = pixel_color / ((float) PATH_TRACING_MAX_SAMPLES);
                camera.set_raw_pixel_clr(x, y, pixel_color + camera.get_raw_pixel_clr(x, y));
            }
        }

        std::cout << "Path tracing iteration number: " <<  pathtracing_sample + 1 << ", time elapsed: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << std::endl;
        CreateImage(pathtracing_sample + 1);
    }
}

double clamp(float input, float min, float max) {
    if (input < min)
        return min;

    if (input > max)
        return max;

    return input;
}

void Renderer::CreateImage(int pathtracing_iteration) {
    double max_magnitude = 1.0;
//    double current_max;
//    for (int x = 0; x < Camera::CAMERA_WIDTH; x++) {
//        for (int y = 0; y < Camera::CAMERA_HEIGHT; y++) {
//            current_max = camera.get_raw_pixel_clr(x,y).get_clr_magnitude();
//            if (current_max > max_magnitude) {
//                max_magnitude = current_max;
//            }
//        }
//    }
//    max_magnitude /= ((double) pathtracing_iteration);

    FILE *fp = fopen("render2.ppm", "wb"); /* b - binary mode */
    (void) fprintf(fp, "P6\n%d %d\n255\n", Camera::CAMERA_WIDTH, Camera::CAMERA_HEIGHT);
    static unsigned char color[3];
    double temp_r, temp_g, temp_b;
    for (int y = 0; y < Camera::CAMERA_WIDTH; y++) {
        for (int x = 0; x < Camera::CAMERA_HEIGHT; x++) {
            ColorRGB pixel_color = camera.get_raw_pixel_clr(x, y) / ((float) pathtracing_iteration * max_magnitude);

            temp_r = pow(clamp(pixel_color.get_rgb().r, 0.0f, 1.0f), 0.5f);
            temp_g = pow(clamp(pixel_color.get_rgb().g, 0.0f, 1.0f), 0.5f);
            temp_b = pow(clamp(pixel_color.get_rgb().b, 0.0f, 1.0f), 0.5f);
            //max_magnitude = sqrt(1.0); // in case max != 1

            color[0] = (unsigned char) (temp_r * 255.99);
            color[1] = (unsigned char) (temp_g * 255.99);
            color[2] = (unsigned char) (temp_b * 255.99);

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
