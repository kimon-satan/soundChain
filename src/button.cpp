#include "button.h"

button::button()
{

    ofRectangle r = m_pathContainer.getBoundsO();

    //ctor
    m_isSelected = false;
    m_radius = 15;
    m_posGlobalO.set(r.width * 0.35,-r.height * 0.35, 0);
    m_posGlobalC.set(m_posGlobalO);

    m_histSize = 10;

    m_pivMin = 0;
    m_pivMax = 90;
    m_pivot.set(-r.width * 0.35, r.height * 0.35);

    m_isSoundOn = false;
    m_isMoving = false;


}

void button::update(){

    m_timeAcc += ofGetLastFrameTime();
    if(m_timeAcc < 1.0/120.0)return; // lock to prevent too fast frame updates
    m_timeAcc = 0;

    m_hist.push_back(ofVec2f(m_posGlobalC.x, m_posGlobalC.y));

    if(m_hist.size() > m_histSize)m_hist.erase(m_hist.begin());

    if(m_hist.size()  > 2){
        float t_dist = 0;
        for(int i = 0; i < m_hist.size() - 1; i ++)t_dist += m_hist[i].distance(m_hist[i + 1]);
        m_isMoving = t_dist > 1;
    }


    //send on moving mode ... other modes to follow

    ofxOscMessage m;

    if(m_isMoving && ! m_isSoundOn){
        m.setAddress("/startS");
        p_sender->sendMessage(m);
        m_isSoundOn = true;
    }else if(!m_isMoving && m_isSoundOn){
        m.setAddress("/stopS");
        p_sender->sendMessage(m);
        m_isSoundOn = false;
    }else if(m_isSoundOn){
        m.setAddress("/updateS"); //TODO add params here
        p_sender->sendMessage(m);
    }

}

void button::draw(){

    ofPushStyle();
    ofSetColor(0);
    (m_isSelected)? ofFill(): ofNoFill();
    ofCircle(m_posGlobalC, m_radius);
    ofPopStyle();

    m_pathContainer.draw();

}

void button::press(ofVec2f t_mouse){

    //mouse coordinates already translated in testApp
    m_isSelected = t_mouse.distance(m_posGlobalC) <= m_radius;

    if(m_isSelected){
        m_posGlobalO = m_posGlobalC; // store last position b4 transform
        m_pathContainer.updateO();
    }


}

void button::drag(ofVec2f t_mouse){

    if(!m_isSelected)return;

    //naive version
    //m_posGlobalC.set(t_mouse);


    //pivot version

    float dist = m_posGlobalC.distance(m_pivot);
    ofVec2f v = m_pivot - t_mouse;
    float angle = -v.angle(ofVec2f(0,1));
    ofVec2f p = m_pivot + ofVec2f(0,-dist);

    angle = min(m_pivMax, max(m_pivMin, angle));
    p.rotate(angle, m_pivot);
    m_posGlobalC.set(p);


    //rotate the path
    float ang_moved = ofVec2f(m_pivot - m_posGlobalO).angle(ofVec2f(m_pivot - m_posGlobalC)); //how much moved
    m_pathContainer.rotateC(ang_moved ,m_pivot);

}

void button::release(){

    m_isSelected = false;

}

ofVec2f button::getModMouse(){
    return m_posGlobalC;
}

bool button::getIsSelected(){
    return m_isSelected;
}

void button::setOSCSender( ofPtr<ofxOscSender> o){
    p_sender = o;
}


button::~button()
{
    //dtor
}
