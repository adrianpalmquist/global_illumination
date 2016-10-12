//
// Created by Filip Kantedal on 2016-10-11.
//

#ifndef TNCG15_PROJ_PHOTONOCTREE_H
#define TNCG15_PROJ_PHOTONOCTREE_H


#include <vector>
#include <queue>
#include "BoundingBox.h"
#include "Photon.h"

class PhotonOctree {
public:
    PhotonOctree();
    PhotonOctree(BoundingBox region, std::vector<Photon*> photons);

    void BuildTree();
    std::vector<Photon*> GetIntersections(vec3 position);

    void set_parent(PhotonOctree* _parent);
    std::vector<Photon*> get_photons();
private:
    BoundingBox region;
    std::vector<Photon*> photons;
    PhotonOctree* child_nodes[8];
    char active_nodes = 0;
    PhotonOctree* parent;

    PhotonOctree* CreateNode(BoundingBox _region, std::vector<Photon*> _photons);
};


#endif //TNCG15_PROJ_PHOTONOCTREE_H
