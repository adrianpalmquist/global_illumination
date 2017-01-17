//
// Created by Filip Kantedal on 2016-10-11.
//

#ifndef TNCG15_PROJ_PHOTONOCTREE_H
#define TNCG15_PROJ_PHOTONOCTREE_H


#include <vector>
#include <queue>
#include <iostream>
#include "../scene/BoundingBox.h"
#include "Photon.h"

class PhotonOctree {
public:
    PhotonOctree();
    PhotonOctree(BoundingBox region, std::vector<Photon*> photons, int _depth);

    void BuildTree();
    std::vector<Photon*> GetIntersections(vec3 position);

    void set_parent(PhotonOctree* _parent);
    std::vector<Photon*> get_photons();

    void displayTree() {
        for (int a = 0; a < 8; a++) {
            if (active_nodes & 1 << a)
                std::cout << "Depth: " << depth << " Photons: " << child_nodes[a]->get_photons().size() << std::endl;
        }

        for (int a = 0; a < 8; a++) {
            if (active_nodes & 1 << a)
                child_nodes[a]->displayTree();
        }
    }
private:
    BoundingBox region;
    std::vector<Photon*> photons;
    PhotonOctree* child_nodes[8];
    char active_nodes = 0;
    int depth;
    PhotonOctree* parent;

    PhotonOctree* CreateNode(BoundingBox _region, std::vector<Photon*> _photons);
};


#endif //TNCG15_PROJ_PHOTONOCTREE_H
