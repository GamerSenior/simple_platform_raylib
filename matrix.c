//
// Created by Giovani Garcia on 17/12/2019.
//
#include "vendor/Chipmunk2D/include/chipmunk/chipmunk.h"

cpVect translateVector(cpVect from, cpVect to) {
    cpFloat transMatrix[3][3] = {
            1, 0, to.x,
            0, 1, to.y,
            0, 0, 1
    };
    cpFloat fromVector[1][3] = {
            from.x,
            from.y,
            1
    };
    cpFloat result[1][3];

    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                result[i][j] += fromVector[i][k] * transMatrix[k][j];
            }
        }
    }

    return (cpVect) {result[0][0], result[0][1]};
}