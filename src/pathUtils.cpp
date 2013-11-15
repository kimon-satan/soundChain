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

    float min_neg[2] = {-2 * PI, -2 * PI};
    float min_pos[2] = {PI, PI};
    int negP[2] = {-1,-1};
    int posP[2] = {-1,-1};

    for(int j = 0; j < 2; j ++) {

        ofVec2f sv = (j == 0) ? uv : -uv;

        ofRectangle r;

        ofVec2f s(sv/ofVec2f(abs(sv.x), abs(sv.y))); //just the signs

        r.set(p - s * 5, ofPoint(ofGetWidth()/2, ofGetHeight()/2) * s);

        if(uv.x == 0)r.set(ofPoint(- ofGetWidth()/2 , ofGetHeight()/2 * s.y) , ofPoint( ofGetWidth()/2, p.y));
        if(uv.y == 0)r.set(ofPoint( ofGetWidth()/2 * s.x, - ofGetHeight()/2 ) , ofPoint( p.x, ofGetHeight()/2 ));

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
            ofVec2f ip = (t_verts[posP[j]] + t_verts[negP[j]])/2.0f; //not very accurate ... don't be lazy find the intersect
            intersects.push_back(ip);
        }



    }

    return intersects;

}


void setBoundsFromPath(ofPolyline & tPoly, vector<ofVec2f> & bnds, ofVec2f pos, ofVec2f v, int bufPixels){


    bnds = getIntersects(tPoly, v, pos);

    ofVec2f t_vecs[2];

    for(int i = 0 ; i < 2 ; i++){
        t_vecs[i] = pos - bnds[i];
        t_vecs[i] = t_vecs[i].getLimited(t_vecs[i].length() - bufPixels);
        bnds[i] = pos + t_vecs[i];
    }

}

void setBoundsFromPath(ofPolyline & tPoly, vector<ofVec2f> & bnds, ofVec2f pos, ofVec2f v, float prop){

    bnds = getIntersects(tPoly, v, pos);
    int bufPixels = ofVec2f(bnds[1] - bnds[0]).length() * (1 - prop) * 0.5;
    ofVec2f t_vecs[2];

    for(int i = 0 ; i < 2 ; i++){
        t_vecs[i] = pos - bnds[i];
        t_vecs[i] = t_vecs[i].getLimited(t_vecs[i].length() - bufPixels);
        bnds[i] = pos + t_vecs[i];
    }

}





}
