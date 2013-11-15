#ifndef PATHUTILS_H
#define PATHUTILS_H

#include "ofMain.h"

namespace pathUtils
{

    void createRoundedRect(ofPolyline & tPoly, ofPoint center, int width, int height, float roundedCorners, int spacing);
    vector<ofVec2f> getIntersects(ofPolyline & pl, ofVec2f uv, ofVec2f p);
    void setBoundsFromPath(ofPolyline & tPoly, vector<ofVec2f> & bnds, ofVec2f pos, ofVec2f v, float prop);
    void setBoundsFromPath(ofPolyline & tPoly, vector<ofVec2f> & bnds, ofVec2f pos, ofVec2f v, int bufPixels);

};

#endif // PATHUTILS_H
