//
// Created by Filip Kantedal on 2016-10-06.
//

#ifndef TNCG15_PROJ_PHOTONKDTREE_H
#define TNCG15_PROJ_PHOTONKDTREE_H


#include "Photon.h"

class PhotonKdTree {
public:
    PhotonKdTree();

    void Insert(Photon* _photon);
private:
    struct KdNode {
        Photon* photon;
        KdNode* left;
        KdNode* right;
    };
};


#endif //TNCG15_PROJ_PHOTONKDTREE_H
