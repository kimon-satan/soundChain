#include "pathContainer.h"

pathContainer::pathContainer(){

    m_corners = 0.25;
    m_spacing = 3;
    m_posC.set(0,0,0);
    m_rotC = 0;
    pathUtils::createRoundedRect(m_polyC, ofPoint(0,0,0), 200, 200, m_corners, m_spacing);
    updateO();

}

void pathContainer::updateO() {

    m_polyO = m_polyC; // incase of resize or resample

    m_rotO = m_rotC;
    m_posO = m_posC;

    vector<ofPoint> t_verts = m_polyO.getVertices();
    for(int i = 0; i < t_verts.size(); i++){
        t_verts[i].rotate(m_rotC, ofVec3f(0,0,1));
        t_verts[i] += m_posC;
    }

    m_polyA.clear();
    m_polyA.addVertices(t_verts);

}


void pathContainer::resizeC(int width, int height) {

    pathUtils::createRoundedRect(m_polyO,
                                 m_polyO.getCentroid2D(),
                                 width, height,
                                 m_corners, m_spacing);

    m_polyC = m_polyO; // need to think how to handle translation etc.

}

void pathContainer::rotateMan(float angle, ofVec2f pivot) {

    m_rotC += angle;
    m_posC = m_posC.getRotated(angle, pivot, ofVec3f(0,0,1));

}

void pathContainer::translateMan(ofVec2f v){ m_posC += v;}

void pathContainer::translateAuto(ofVec2f t_vec, vector<ofVec2f> bounds){

    t_vec *= ofGetLastFrameTime(); //spped in pixels per second ...
    ofVec2f p = m_posC;
    p += t_vec;

    bool isLimit = false;

    if(t_vec.x < 0  && p.x < min(bounds[0].x, bounds[1].x)){isLimit = true;}
    if(t_vec.x > 0 && p.x > max(bounds[0].x, bounds[1].x)){isLimit = true;}
    if(t_vec.y < 0 && p.y < min(bounds[0].y, bounds[1].y)){isLimit = true;}
    if(t_vec.y > 0 && p.y > max(bounds[0].y, bounds[1].y)){isLimit = true;}

    if(!isLimit) m_posC.set(p);


}

vector<ofVec2f> pathContainer::getIntersects(ofVec2f uv, ofVec2f p){
    return pathUtils::getIntersects(m_polyA, uv, p);
}

ofVec2f pathContainer::localToWorldPoint(ofVec2f local){

    ofVec2f p;
    ofRectangle r = m_polyC.getBoundingBox();
    p.set(m_posC.x + r.width * local.x, m_posC.y + r.height * local.y);
    p.rotate(m_rotC, m_posC);
    return p;
}

float pathContainer::localToWorldRot(float local){
    float r = local + m_rotC;
    while(r > 180)r -= 360;
    while(r < -180)r += 360;
    return r;
}

ofVec2f pathContainer::localToWorldVec(ofVec2f local){
    return local.rotate(m_rotC);
}

void pathContainer::draw() {


    ofPushStyle();
    ofSetLineWidth(2);
    ofSetColor(0);
        ofPushMatrix();
        ofTranslate(m_posC.x, m_posC.y, m_posC.z);
        ofRotate(m_rotC, 0, 0, 1);
        m_polyC.draw();

        //debug draw
        /*vector<ofPoint> t_verts = m_polyC.getVertices();
        for(int i = 0; i < t_verts.size(); i++){
            ofCircle(t_verts[i],1);
        }*/

        ofPopMatrix();


    ofPopStyle();

}



ofRectangle pathContainer::getBoundsO(){
    return m_polyO.getBoundingBox();
}


ofPoint pathContainer::getPos(){ return m_posC; }
float pathContainer::getRot(){ return m_rotC; }

pathContainer::~pathContainer() {
    //dtor
}
