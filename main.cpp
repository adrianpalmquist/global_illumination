#include <iostream>
#include <time.h>
#include "Renderer.h"

vec3 reflect(vec3 normal) {
    double azimuth = 2 * 3.14 * ((double) rand() / RAND_MAX); //phi
    double inclination = acos(sqrt((double) rand() / RAND_MAX)); //theta

    vec3 reflected_dir = normalize(vec3(sin(inclination) * cos(azimuth), sin(inclination) * sin(azimuth), cos(inclination)));
    vec3 tangent_1 = cross(normal, vec3(0,0,1));
    vec3 tangent_2 = cross(normal, tangent_1);

    mat3 rotationMatrix = mat3(tangent_1, tangent_2, normal);
    reflected_dir = transpose(rotationMatrix)*reflected_dir;

    return normalize(reflected_dir);
}

vec3 CosineDistributeDirection(vec3 normal) {
    float sin_theta = (((float) rand()) / RAND_MAX);
    float cos_theta = (float) (1.0 - sin_theta * sin_theta);

    // Random in plane angle;
    float psi = (float) (((float) rand()) / RAND_MAX * 2.0f * M_PI);

    // Generate tangents along plane
    vec3 tangent1, tangent2;
    tangent1 = cross(normal, vec3(0,0,1));
    tangent2 = cross(normal, tangent1);

    vec3 v1, v2, v3;
    v1 = sin_theta * cos(psi) * tangent1;
    v2 = sin_theta * sin(psi) * tangent2;
    v3 = cos_theta * normal;

    return v1 + v2 + v3;
}

vec3 rotateVec(vec3 normal) {
    int abortCounter = 0;
    while(true)
    {
        vec3 b((((float) rand()) / RAND_MAX) - 0.5f, (((float) rand()) / RAND_MAX) - 0.5f, (((float) rand()) / RAND_MAX) - 0.5f);
        b = normalize(b);

        if (dot(b, normal) > 0)
        {
            return b;
        }

        abortCounter++;
        if (abortCounter > 500)
        {
            return b; // for some reason (NaN's perhaps) we don't found a normal
        }
    }
}

int main() {
    srand (time(NULL));

    const clock_t begin_time = clock();

//    vec3 test(1, 0, 1);
//    vec3 mean_direction = vec3(0,0,0);
//    float iterations = 1000000.0f;
//    for (int i = 0; i < iterations; i++) {
//        //vec3 reflection = reflect(test);
//        vec3 reflection = rotateVec(test);
//        mean_direction += reflection;
//        //std::cout << reflection.x << " " << reflection.y << " " << reflection.z << std::endl;
//    }
//    mean_direction /= iterations;
//
//    std::cout << "mean mean" << std::endl;
//    std::cout << mean_direction.x << " " << mean_direction.y << " " << mean_direction.z << std::endl;

    Renderer r = Renderer();
    r.Render();

    std::cout << "Render complete, time elapsed: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << std::endl;
    return 0;
}