//
// Created by Filip Kantedal on 13/09/16.
//

#pragma once

#include <math.h>
#include "Pixel.h"
#include <stdio.h>
#include "glm/glm.hpp"
#include "Scene.h"

using namespace glm;

class Camera {
public:
    static const int CAMERA_WIDTH = 600;
    static const int CAMERA_HEIGHT = 600;
    static const int EYE1 = 0;
    static const int EYE2 = 2;

    Camera();

    vec3 get_camera_position();
    std::vector<vec3> get_camera_direction(int x, int y);
    void set_pixel_clr(int x, int y, ColorDbl clr);
    ColorDbl get_pixel_clr(int x, int y);

private:
    vec3 camera_position;
    vec3 look_at;
    double field_of_view = 3.14/4.0 ;
    bool selected_eye;

    //Camera plane size is 2x2
    Pixel** camera_plane;

    // Camera plane corner vertices
    vec3 camera_v1;
    vec3 camera_v2;
    vec3 camera_v3;
    vec3 camera_v4;

    // Function to retrieve pixels position on the camera plane
    vector<vec3> GetPixelPositions(int x, int y);

    // Function to calculate camera plane
    void GetCameraPlane();
};

Camera::Camera() {
    // Initiate eye positions
    camera_position = vec3(-1, -0, 0);

    look_at = vec3(1,0,0);

    camera_plane = new Pixel*[CAMERA_WIDTH];
    for (int i = 0; i < CAMERA_WIDTH; ++i) {
        camera_plane[i] = new Pixel[CAMERA_HEIGHT];
    }

    GetCameraPlane();
}

std::vector<vec3> Camera::GetPixelPositions(int x, int y) {
    std::vector<vec3> positions;
    vec3 base_vector_x = vec3(camera_v3) - vec3(camera_v4);
    vec3 base_vector_y = vec3(camera_v1) - vec3(camera_v4);
    vec3 dx = base_vector_x / (float) CAMERA_WIDTH;
    vec3 dy = base_vector_y / (float) CAMERA_HEIGHT;
    vec3 vert1 = vec3(camera_v4) + (float) x / (float) CAMERA_WIDTH * base_vector_x + (float) y / (float) CAMERA_HEIGHT * base_vector_y;
    vec3 vert2 = vert1 + dx/(float)2.0;
    vec3 vert3 = vert2 + dx/(float)2.0;
    vec3 vert4 = vert3 + dy/(float)2.0;
    vec3 vert5 = vert4 + dy/(float)2.0;
    vec3 vert6 = vert5 - dx/(float)2.0;
    vec3 vert7 = vert6 - dx/(float)2.0;
    vec3 vert8 = vert7 - dy/(float)2.0;
    vec3 vert9 = vert8 + dx/(float)2.0;

    vec3 vect1 = vert2 - vert1;
    vec3 vect2 = vert8 - vert1;
    vec3 vect3 = vert3 - vert2;
    vec3 vect4 = vert9 - vert2;
    vec3 vect5 = vert4 - vert9;
    vec3 vect6 = vert6 - vert9;
    vec3 vect7 = vert9 - vert8;
    vec3 vect8 = vert7 - vert8;

    vec3 position1 = vect1*(float)rand() + vect2*(float)rand();
    vec3 position2 = vect3*(float)rand() + vect4*(float)rand();
    vec3 position3 = vect5*(float)rand() + vect6*(float)rand();
    vec3 position4 = vect7*(float)rand() + vect8*(float)rand();


    positions.push_back(position1);
    positions.push_back(position2);
    positions.push_back(position3);
    positions.push_back(position4);

    return positions;
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
    std::vector<vec3> sampled_positions = GetPixelPositions(x, y);
    for (int i = 0; i < sampled_positions.size(); i++) {
        sampled_positions.at(i) = normalize(sampled_positions.at(i) - camera_position);
    }
    return sampled_positions;
}

void Camera::set_pixel_clr(int x, int y, ColorDbl clr) {
    camera_plane[y][x].set_clr(clr);
}

ColorDbl Camera::get_pixel_clr(int x, int y) {
    return camera_plane[x][y].get_clr();
}
