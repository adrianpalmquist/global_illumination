//
// Created by Filip Kantedal on 23/09/16.
//

#include <iostream>
#include "Camera.h"
#include "Renderer.h"


Camera::Camera() {
    // Initiate eye positions
    camera_position = vec3(-1, -0, 0);
    field_of_view = 3.14f / 4.0f;
    look_at = vec3(1,0,0);

    raw_camera_plane = new Pixel*[CAMERA_WIDTH];
    for (int i = 0; i < CAMERA_WIDTH; ++i) {
        raw_camera_plane[i] = new Pixel[CAMERA_HEIGHT];
    }

    for (int y = 0; y < Camera::CAMERA_HEIGHT; y++) {
        for (int x = 0; x < Camera::CAMERA_WIDTH; x++) {
            set_raw_pixel_clr(x, y, ColorRGB(0, 0, 0));
        }
    }

    GetCameraPlane();
}

std::vector<vec3> Camera::GetPixelPositions(int x, int y) {
    std::vector<vec3> positions;

    vec3 base_vector_x = vec3(camera_v3) - vec3(camera_v4);
    vec3 base_vector_y = vec3(camera_v1) - vec3(camera_v4);

    vec3 dx = (base_vector_x / (float) CAMERA_WIDTH) / ((float) sqrt(Renderer::ANTI_ALIASING_SAMPLES));
    vec3 dy = (base_vector_y / (float) CAMERA_HEIGHT) / ((float) sqrt(Renderer::ANTI_ALIASING_SAMPLES));

    for (int sample_x = 0; sample_x < 2; sample_x++) {
        for (int sample_y = 0; sample_y < 2; sample_y++) {
            vec3 rand_x = dx * ((float) rand()/RAND_MAX);
            vec3 rand_y = dy * ((float) rand()/RAND_MAX);

            vec3 vert = vec3(camera_v4) + ((float) x + (sample_x / 2.0f)) / (float) CAMERA_WIDTH * base_vector_x
                        + ((float)y + (sample_y / 2.0f)) / (float) CAMERA_HEIGHT * base_vector_y;
            vert += rand_x + rand_y;
            positions.push_back(normalize(vert - camera_position));
        }
    }

    return positions;
}

vec3 Camera::GetPixelPosition(int x, int y) {
    vec3 base_vector_x = vec3(camera_v3) - vec3(camera_v4);
    vec3 base_vector_y = vec3(camera_v1) - vec3(camera_v4);
    vec3 vert = vec3(camera_v4) + (float) x / (float) CAMERA_WIDTH * base_vector_x + (float) y / (float) CAMERA_HEIGHT * base_vector_y;
    return normalize(vert - camera_position);
}


void Camera::GetCameraPlane() {
    float d = (float) (0.5 / tan(field_of_view/2));

    // Initiate camera plane corner vertices
    camera_v1 = vec3(0, -1/d, -1/d);
    camera_v2 = vec3(0, 1/d, -1/d);
    camera_v3 = vec3(0, 1/d, 1/d);
    camera_v4 = vec3(0, -1/d, 1/d);
}

vec3 Camera::get_camera_position() {
    return camera_position;
}

std::vector<vec3> Camera::get_camera_direction(int x, int y) {
    // Check if anti aliasing is enabled
    if (Renderer::ANTI_ALIASING) {
        return GetPixelPositions(x,y);
    }
    else {
        std::vector<vec3> sampled_positions;
        sampled_positions.push_back(GetPixelPosition(x,y));
        return sampled_positions;
    }
}

void Camera::set_raw_pixel_clr(int x, int y, ColorRGB clr) {
    raw_camera_plane[x][y].set_clr(clr);
}

ColorRGB Camera::get_raw_pixel_clr(int x, int y) {
    return raw_camera_plane[x][y].get_clr();
}

