#include "pathUtils.h"

namespace pathUtils {

void createRoundedRect(ofPolyline & tPoly, ofPoint center, int width, int height, float roundedCorners, int spacing) {

    float r = height * roundedCorners;
    ofPoint p;

    //start point
    p = center + ofVec3f(-width/2 + r, height/2);
    tPoly.addVertex(p);

    //top right
    p = center + ofVec3f(width/2 - r, height/2 - r);
    tPoly.arc(p, r, r, 90,0, false, 2 * r * PI);

    //bottom right
    p = center + ofVec3f(width/2 - r, -height/2 + r);
    tPoly.arc(p, r,r, 0,-90, false, 2 * r * PI);

    //bottom left
    p = center + ofVec3f(-width/2 + r, -height/2 + r);
    tPoly.arc(p,r,r, -90,-180, false, 2 * r * PI);

    //top left
    p = center + ofVec3f(-width/2 + r, height/2 - r);
    tPoly.arc(p, r,r, -180,90, false, 2 * r * PI);

    tPoly.close();

    //resample
    tPoly = tPoly.getResampledBySpacing(spacing);

}


}
