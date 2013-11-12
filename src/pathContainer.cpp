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

    m_rotC = m_rotO + angle;
    m_posC = m_posO.getRotated(angle, pivot, ofVec3f(0,0,1));

}

void pathContainer::translateMan(ofVec2f v){m_posC = m_posO + v;}

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


void pathContainer::setBounds(ofVec2f pos, ofVec2f v, float prop){
    setBoundsFromPath(m_bounds, pos,v,prop);
}

void pathContainer::setBounds(ofVec2f pos, ofVec2f v, int bufPixels){
    setBoundsFromPath(m_bounds, pos,v,bufPixels);
}

void pathContainer::setBoundsFromPath(vector<ofVec2f> & bnds, ofVec2f pos, ofVec2f v, int bufPixels){

    //might make more sense in path

    bnds = getIntersects(v, pos);
    ofVec2f t_vecs[2];

    for(int i = 0 ; i < 2 ; i++){
        t_vecs[i] = pos - bnds[i];
        t_vecs[i] = t_vecs[i].getLimited(t_vecs[i].length() - bufPixels);
        bnds[i] = pos + t_vecs[i];
    }

}

void pathContainer::setBoundsFromPath(vector<ofVec2f> & bnds, ofVec2f pos, ofVec2f v, float prop){

    bnds = getIntersects(v, pos);
    int bufPixels = ofVec2f(bnds[1] - bnds[0]).length() * (1 - prop) * 0.5;
    ofVec2f t_vecs[2];

    for(int i = 0 ; i < 2 ; i++){
        t_vecs[i] = pos - bnds[i];
        t_vecs[i] = t_vecs[i].getLimited(t_vecs[i].length() - bufPixels);
        bnds[i] = pos + t_vecs[i];
    }

}


ofRectangle pathContainer::getDims(){
    return m_polyO.getBoundingBox();
}


ofPoint pathContainer::getPos(){ return m_posC; }
float pathContainer::getRot(){ return m_rotC; }

vector<ofVec2f> pathContainer::getBounds(){ return m_bounds; }

pathContainer::~pathContainer() {
    //dtor
}
