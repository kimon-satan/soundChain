#include "vessel.h"

vessel::vessel(){

    m_corners = 0.25;
    m_spacing = 3;
    m_posC.set(0,0,0);
    m_rotC = 0;
    pathUtils::createRoundedRect(m_polyC, ofPoint(0,0,0), 200, 200, m_corners, m_spacing);
    updateO();

}


void vessel::updateO() {

    m_polyO = m_polyC; // incase of resize or resample

    vector<ofPoint> t_verts = m_polyO.getVertices();
    for(int i = 0; i < t_verts.size(); i++){
        t_verts[i].rotate(m_rotC, ofVec3f(0,0,1));
        t_verts[i] += m_posC;
    }

    m_polyA.clear();
    m_polyA.addVertices(t_verts);


}

void vessel::draw() {

    ofPushStyle();
    ofSetLineWidth(2);
    ofSetColor(0);
        ofPushMatrix();
        ofTranslate(m_posC.x, m_posC.y, m_posC.z);
        ofRotate(m_rotC, 0, 0, 1);
        m_polyC.draw();
        ofPopMatrix();
    ofPopStyle();

}




ofVec2f vessel::localToWorldPoint(ofVec2f local){

    ofVec2f p;
    ofRectangle r = m_polyC.getBoundingBox();
    p.set(m_posC.x + r.width * local.x, m_posC.y + r.height * local.y);
    p.rotate(m_rotC, m_posC);
    return p;
}

float vessel::localToWorldRot(float local){
    float r = local + m_rotC;
    while(r > 180)r -= 360;
    while(r < -180)r += 360;
    return r;
}

ofVec2f vessel::localToWorldVec(ofVec2f local){
    return local.rotate(m_rotC);
}


ofRectangle vessel::getDims(){
    return m_polyO.getBoundingBox();
}


ofPoint vessel::getPos(){ return m_posC; }
void vessel::setPos(ofPoint p){ m_posC.set(p); }

float vessel::getRot(){ return m_rotC; }
void vessel::setRot(float f){m_rotC = f;}





vessel::~vessel() {
    //dtor
}
