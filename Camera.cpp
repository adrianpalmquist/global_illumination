//
// Created by Filip Kantedal on 23/09/16.
//

#include "Camera.h"


Camera::Camera() {
    // Initiate eye positions
    camera_position = vec3(-1, -0, 0);
    field_of_view = 3.14f / 4.0f;
    look_at = vec3(1,0,0);

    camera_plane = new Pixel*[CAMERA_WIDTH];
    for (int i = 0; i < CAMERA_WIDTH; ++i) {
        camera_plane[i] = new Pixel[CAMERA_HEIGHT];
    }

    GetCameraPlane();
}

vec3 Camera::GetPixelPosition(int x, int y) {
    vec3 base_vector_x = vec3(camera_v3) - vec3(camera_v4);
    vec3 base_vector_y = vec3(camera_v1) - vec3(camera_v4);
    return vec3(camera_v4) + (float) x / (float) CAMERA_WIDTH * base_vector_x + (float) y / (float) CAMERA_HEIGHT * base_vector_y;
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

vec3 Camera::get_camera_direction(int x, int y) {
    return normalize(GetPixelPosition(x,y) - camera_position);
}

void Camera::set_pixel_clr(int x, int y, ColorDbl clr) {
    camera_plane[y][x].set_clr(clr);
}

ColorDbl Camera::get_pixel_clr(int x, int y) {
    return camera_plane[x][y].get_clr();
}