#include <iostream>
#include <time.h>
#include "Renderer.h"
#include "material/DiffuseMaterial.h"

void TransmitRay(vec3 direction, vec3 collision_normal, vec3 reflected_direction, float &radiance_dist, bool incoming_ray, vec3 &transmitted_direction) {
    float n1, n2;
    if (!(collision_normal.x == 0 && collision_normal.y == 0 && collision_normal.z == 0)) {
        vec3 real_normal = incoming_ray ? collision_normal : -collision_normal;

        if (incoming_ray) {
            n1 = 1.0f;
            n2 = 1.5f;
        }
        else {
            n1 = 1.5f;
            n2 = 1.0f;
        }

        float cosI = dot(direction, real_normal);
        float n = n1 / n2;
        float sinT2 = n*n * (1.0f - cosI * cosI);
        float cosT = sqrtf(1.0 - sinT2);

        // Fresnel equations
        float rn = (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT);
        float rt = (n2 * cosI - n1 * cosT) / (n2 * cosI + n2 * cosT);
        rn *= rn;
        rt *= rt;
        float refl = (rn + rt) * 0.5f;
        float trans = 1.0f - refl;

        if (n == 1.0) {
            transmitted_direction = direction;
            return;
        }


        if(cosT * cosT < 0.0)//tot inner refl
        {
            refl = 1;
            trans = 0;
            transmitted_direction = direction;
            return;
        }

        transmitted_direction =  (n * cosI - cosT) * real_normal;
    }
}


int main() {
    srand (time(NULL));
    const clock_t begin_time = clock();

//    Sphere test(vec3(0,0,0), 1, new DiffuseMaterial(ColorRGB(1,1,1)));
//
//    vec3 collision_pos;
//    vec3 collision_normal;
//    vec3 origin = vec3(-2, 0, 0);
//    vec3 direction = vec3(1, 0, 0);
//    float radiance_dist = 0.0f;
//
//    vec3 transmitted_direction;
//    test.RayIntersection(origin, direction, collision_pos, collision_normal);
//    TransmitRay(direction, collision_normal, vec3(0), radiance_dist, true, transmitted_direction);
//
//    std::cout << "Collision pos 1: " <<  collision_pos.x << " " << collision_pos.y << " " << collision_pos.z << std::endl;
//    std::cout << "Collision normal 1: " <<  collision_normal.x << " " << collision_normal.y << " " << collision_normal.z << std::endl;
//    std::cout << "Transmitted direction 1: " << transmitted_direction.x << " " << transmitted_direction.y << " " << transmitted_direction.z << std::endl;
//
//    test.RayIntersection(vec3(collision_pos), direction, collision_pos, collision_normal);
//    TransmitRay(direction, collision_normal, vec3(0), radiance_dist, false, transmitted_direction);
//
//    std::cout << std::endl;
//    std::cout << "Collision pos 2: " <<  collision_pos.x << " " << collision_pos.y << " " << collision_pos.z << std::endl;
//    std::cout << "Collision normal 2: " <<  collision_normal.x << " " << collision_normal.y << " " << collision_normal.z << std::endl;
//    std::cout << "Transmitted direction 2: " << transmitted_direction.x << " " << transmitted_direction.y << " " << transmitted_direction.z << std::endl;
//    std::cout << std::endl;


    Renderer r = Renderer();
    r.Render();

    std::cout << "Render complete, time elapsed: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << std::endl;
    return 0;
}