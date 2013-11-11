#include "pathUtils.h"

namespace pathUtils {

void createRoundedRect(ofPolyline & tPoly, ofPoint center, int width, int height, float roundedCorners, int spacing) {

    float r = height * roundedCorners;
    ofPoint p;

    //start point
    p = center + ofVec3f(0, height/2);
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

vector<ofVec2f> getIntersects(ofPolyline & pl, ofVec2f uv, ofVec2f p) {

    vector<ofPoint> t_verts = pl.getVertices();
    vector<ofVec2f> intersects;

    uv.normalize();
   // uv.x = abs(uv.x); uv.y = abs(uv.y); // make unsigned

    float min_neg[2] = {-PI, -PI};
    float min_pos[2] = {PI, PI};
    int negP[2] = {0,0};
    int posP[2] = {0, 0};

    for(int j = 0; j < 2; j ++) {

        ofVec2f sv = (j == 0) ? uv : -uv;

        ofRectangle r;

        ofVec2f s(sv/ofVec2f(abs(sv.x), abs(sv.y))); //just the signs
        r.set(p - s * 5, ofPoint(ofGetWidth()/2, ofGetHeight()/2) * s);

        if(sv.x == 0)r.set(ofPoint(- ofGetWidth()/2 , ofGetHeight()/2 * s.y) , ofPoint( ofGetWidth()/2, p.y));
        if(sv.y == 0)r.set(ofPoint( ofGetWidth()/2 * s.x, - ofGetHeight()/2 ) , ofPoint( p.x, ofGetHeight()/2 ));

        for(int i = 0; i < t_verts.size(); i ++) {

            if(!r.inside(t_verts[i]))continue;

            ofVec2f v = (t_verts[i] - p).getNormalized();
            float f = v.angleRad(sv);


            if(f == 0) {
                negP[j] = i;
                posP[j] = i;
                break;
            } else {

                if(f < 0 && f >= min_neg[j]) {
                    min_neg[j] = f;
                    negP[j] = i;
                } else if(f > 0 && f <= min_pos[j]) {
                    min_pos[j] = f;
                    posP[j] = i;
                }
            }

        }

        if(posP[j] == negP[j]) {
            intersects.push_back(t_verts[negP[j]]);
        } else {
            float a = (float)(posP[j] + negP[j])/2.0f;
            intersects.push_back(pl.getPointAtIndexInterpolated(a));
        }



    }

    return intersects;

}



}
