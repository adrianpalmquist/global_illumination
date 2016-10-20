//
// Created by Filip Kantedal on 2016-10-11.
//

#include <iostream>
#include "PhotonOctree.h"
#include "../Renderer.h"

PhotonOctree::PhotonOctree() {
    photons = std::vector<Photon*>();
    region = BoundingBox();
}

PhotonOctree::PhotonOctree(BoundingBox _region, std::vector<Photon*> _photons, int _depth) {
    region = _region;
    photons = _photons;
    depth = _depth;

    for (int a = 0; a < 8; a++) {
        child_nodes[a] = nullptr;
    }
}

static bool pred(Photon * p) {
    if (p->should_delete()) {
        p->restore_deletion();
        return true;
    }

    return false;
}

void PhotonOctree::BuildTree() {
    if (photons.size() <= 1)
        return;

    vec3 region_min = region.get_min();
    vec3 region_max = region.get_max();
    vec3 dimensions = region.get_max() - region.get_min();

    // Check to see if the dimensions of the box are greater than photon radius
    if (dimensions.x <= 0.4 && dimensions.y <= 0.4 && dimensions.z <= 0.4)
        return;

    vec3 half = dimensions / 2.0f;
    vec3 center = region.get_min() + half;

    // Create subdivided regions for each box
    BoundingBox octant[8];
    octant[0] = BoundingBox(region_min, center);
    octant[1] = BoundingBox(vec3(center.x, region_min.y, region_min.z), vec3(region_max.x, center.y, center.z));
    octant[2] = BoundingBox(vec3(center.x, region_min.y, center.z), vec3(region_max.x, center.y, region_max.z));
    octant[3] = BoundingBox(vec3(region_min.x, region_min.y, center.z), vec3(center.x, center.y, region_max.z));
    octant[4] = BoundingBox(vec3(region_min.x, center.y, region_min.z), vec3(center.x, region_max.y, center.z));
    octant[5] = BoundingBox(vec3(center.x, center.y, region_min.z), vec3(region_max.x, region_max.y, center.z));
    octant[6] = BoundingBox(center, region_max);
    octant[7] = BoundingBox(vec3(region_min.x, center.y, center.z), vec3(center.x, region_max.y, region_max.z));

    // This will contain all of our objects which fit within each respective octant
    std::vector<Photon*> octantList[8];
    for (int i = 0; i < 8; i++) {
        octantList[i] = std::vector<Photon*>();
    }

    // Go through each photon
    for (int i = 0; i < photons.size(); i++) {
        Photon* photon = photons[i];

        for (int a = 0; a < 8; a++) {
            if (octant[a].ConainsPoint(photon->get_position())) {
                octantList[a].push_back(photon);
                photon->mark_deletion();
                break;
            }
        }
    }

    // Remove photons pushed into octants
    photons.erase( std::remove_if( photons.begin(), photons.end(), pred ), photons.end() );

    // Create child nodes where there are items contained in the bounding region
    for (int a = 0; a < 8; a++) {
        if (octantList[a].size() != 0) {
            child_nodes[a] = CreateNode(octant[a], octantList[a]);
            child_nodes[a]->BuildTree();
            active_nodes |= 1 << a;
        }
    }

}

void PhotonOctree::set_parent(PhotonOctree *_parent) {
    parent = _parent;
}

PhotonOctree *PhotonOctree::CreateNode(BoundingBox _region, std::vector<Photon*> _photons) {
    if (_photons.size() == 0)
        return nullptr;
    PhotonOctree *ret = new PhotonOctree(_region, _photons, 1 + depth);
    ret->set_parent(this);
    return ret;
}

float sumsum(vec3 vec) {
    return vec.x + vec.y + vec.z;
}

std::vector<Photon*> PhotonOctree::GetIntersections(vec3 position) {
    std::vector<Photon*> intersecting_photons = std::vector<Photon*>();

    // Check intersections with photons in current node
    for (std::vector<Photon*>::iterator it = photons.begin(); it != photons.end(); ++it) {
        Photon* photon = *it;

        float radius = Renderer::PHOTON_RADIUS;
        float squaredDistance = sumsum((photon->get_position() - position) * (photon->get_position() - position));
        if (squaredDistance < radius * radius) {
            intersecting_photons.push_back(photon);
        }
    }

    // Get intersections with children nodes
    for (int a = 0; a < 8; a++) {
        if (active_nodes & 1 << a) {
            if (child_nodes[a]->region.Contains(position)) {
                std::vector<Photon*> child_intersections = child_nodes[a]->GetIntersections(position);
                if (child_intersections.size() != 0)
                    intersecting_photons.insert(intersecting_photons.end(), child_intersections.begin(), child_intersections.end());
            }
        }
    }

    return intersecting_photons;
}

std::vector<Photon *> PhotonOctree::get_photons() {
    return photons;
}

