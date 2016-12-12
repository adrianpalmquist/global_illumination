//
// Created by Filip Kantedal on 23/09/16.
//

#ifndef TNCG15_PROJ_RAYTRACER_H
#define TNCG15_PROJ_RAYTRACER_H

#include "scene/Scene.h"
#include "Ray.h"
#include "photon_mapping/Photon.h"
#include "photon_mapping/PhotonOctree.h"

class RayTracer {
public:
    RayTracer();
    RayTracer(Scene* _scene);
    void StartRayTracing(Ray base_ray);
    ColorRGB TraceRay(Ray ray, bool perform_full_calc);

    void set_photon_map(PhotonOctree* _photon_map, PhotonOctree* _caustics_photon_map);

private:
    Scene* scene;
    Ray* ray_tree;

    PhotonOctree* photon_map;
    PhotonOctree* caustics_photon_map;

    ColorRGB TraceShadowRays(Ray ray, vec3 collision_point, vec3 surface_normal);
    ColorRGB MeanFromPhotonMap(vec3 position, vec3 object_normal);
    ColorRGB MeanFromCausticsPhotonMap(vec3 position, vec3 object_normal);
};

#endif //TNCG15_PROJ_RAYTRACER_H
