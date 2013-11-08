#include "pathContainer.h"

pathContainer::pathContainer()
{
    m_corners = 0.25;
    m_spacing = 5;
    pathUtils::createRoundedRect(m_polyO, ofPoint(0,0,0), 200, 200, m_corners, m_spacing);
    m_boundsO = m_polyO.getBoundingBox();
    m_polyC = m_polyO;
}



void pathContainer::resizeC(int width, int height){

    pathUtils::createRoundedRect(m_polyO,
                                m_polyO.getCentroid2D(),
                                width, height,
                                m_corners, m_spacing);

    m_boundsO = m_polyO.getBoundingBox();
    m_polyC = m_polyO; // need to think how to handle translation etc.

}

void pathContainer::rotateC(float angle, ofVec2f pivot){

    vector<ofPoint> t_verts = m_polyO.getVertices(); // need to think how to handle translation etc.

    for(int i = 0; i < t_verts.size(); i++){
        t_verts[i].rotate(angle, ofVec3f(pivot), ofVec3f(0,0,1));
    }

    m_polyC.clear();
    m_polyC.addVertices(t_verts);
    m_polyC.close();

}


void pathContainer::draw(){


    ofPushStyle();
    ofSetLineWidth(2);
    m_polyC.draw();
    ofPopStyle();
}

ofRectangle pathContainer::getBoundsO(){
    return m_boundsO;
}

void pathContainer::updateO(){
    m_polyO = m_polyC;
}

pathContainer::~pathContainer()
{
    //dtor
}
