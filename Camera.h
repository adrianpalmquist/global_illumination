//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_CAMERA_H
#define TNCG15_PROJ_CAMERA_H

#include "ColorDbl.h"
#include "Pixel.h"

class Camera {
public:
    static const int CAMERA_WIDTH = 600;
    static const int CAMERA_HEIGHT = 600;

    Camera();
    vec3 get_camera_position();
    std::vector<vec3> get_camera_direction(int x, int y);
    void set_pixel_clr(int x, int y, ColorDbl clr);
    ColorDbl get_pixel_clr(int x, int y);

private:
    vec3 camera_position;
    vec3 look_at;
    float field_of_view;
    bool selected_eye;

    //Camera plane size is 2x2
    Pixel** camera_plane;

    // Camera plane corner vertices
    vec3 camera_v1;
    vec3 camera_v2;
    vec3 camera_v3;
    vec3 camera_v4;

    // Function to retrieve pixels position on the camera plane
    std::vector<vec3> GetPixelPositions(int x, int y);

    // Function to calculate camera plane
    void GetCameraPlane();
};

#endif //TNCG15_PROJ_CAMERA_H
