#include <iostream>
#include "glm/glm.hpp"
#include "ColorDbl.h""
#include "Renderer.h"

using namespace glm;

void glmTest() {
    glm::vec4 Position = glm::vec4(glm::vec3(0.0), 1.0);
    std::cout << Position.x << " " << Position.y << " " << Position.z << " " << Position.w << std::endl;
}

void normalTest() {
    vec3* v1 = new vec3(0,0,1);
    vec3* v2 = new vec3(1,0,0);
    vec3* v3 = new vec3(-1,0,0);
    ColorDbl* clr = new ColorDbl();

    Triangle triangle = Triangle(v1,v2,v3,clr);
   // std::cout << triangle.get_normal() << std::endl;
}

void renderTest() {
//    vec3* v1 = new vec3(0,1,-1);
//    vec3* v2 = new vec3(-1,-1,0);
//    vec3* v3 = new vec3(1,-1,0);
//    ColorDbl* clr = new ColorDbl(1,0,0);
//    Triangle tri(v1, v2, v3, clr);
//
//    vec3 hit;
//    vec3 origin(0,1,2);
//    vec3 dir(0,0,-1);
//
//    vec3 vec;
//
//    tri.rayIntersection(origin, dir, vec);

    Renderer renderer = Renderer();
    renderer.Render();
    renderer.CreateImage();


    //camera.Render(scene);
    //camera.CreateImage();
}

int main() {
    renderTest();

    return 0;
}


