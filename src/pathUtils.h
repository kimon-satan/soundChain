#ifndef PATHUTILS_H
#define PATHUTILS_H

#include "ofMain.h"

namespace pathUtils
{

    void createRoundedRect(ofPolyline & tPoly, ofPoint center, int width, int height, float roundedCorners, int spacing);
    vector<ofVec2f> getIntersects(ofPolyline & pl, ofVec2f uv, ofVec2f p);

};

#endif // PATHUTILS_H
