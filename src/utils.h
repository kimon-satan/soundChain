#ifndef UTILS_H
#define UTILS_H

#include "ofMain.h"

namespace utils
{

    ofVec2f valToVec(float val, vector<ofVec2f> bnds, ofVec2f i_pos);
    float valToRot(float val, float rng, float i_rot);
};

#endif // UTILS_H
