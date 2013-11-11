#include "button.h"

button::button() {

    ofRectangle r = m_pathContainer.getBoundsO();

    //ctor
    m_isSelected = false;
    m_radius = 15;
    m_posGlobalO.set(r.width * 0.35,-r.height * 0.35, 0);
    m_posGlobalC.set(m_posGlobalO);

    m_histSize = 10;

    m_pivMin = 0;
    m_pivMax = 90;

    m_isSoundOn = false;
    m_isMoving = false;

    m_transType = transType(0);
   // m_transType = SNDCHN_TT_TRANS_AUTO;

}

void button::update() {


    m_timeAcc += ofGetLastFrameTime();
    if(m_timeAcc < 1.0/120.0)return; // lock to prevent too fast frame updates
    m_timeAcc = 0;

    calcIsMoving();
    handleOSC();

    if(m_isSelected) {

        if(m_transType == SNDCHN_TT_ROT_AUTO) {

        }


        if(m_transType == SNDCHN_TT_TRANS_AUTO) {

            m_pathContainer.translateAuto(m_transDir, m_transBounds);
        }

    }

}



void button::draw() {

    ofPushStyle();
    ofSetColor(0);
    (m_isSelected)? ofFill(): ofNoFill();
    ofCircle(m_posGlobalC, m_radius);
    ofPopStyle();
    m_pathContainer.draw();

    //debug drawing

    //ofCircle(m_pivot,5);
    if(m_transBounds.size() > 0){
        ofCircle(m_transBounds[0], 5);
        ofCircle(m_transBounds[1], 10);
    }


    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRect(m_pathContainer.getPos(), 6,6);
    ofPopStyle();


}

//mouse interaction

void button::press(ofVec2f t_mouse) {

    //mouse coordinates already translated in testApp
    m_isPointInside = t_mouse.distance(m_posGlobalC) <= m_radius;
    m_isSelected = m_isPointInside;

    if(m_isSelected) {

        m_transType = transType(((int)m_transType + 1)%3); //ultimately this will be replaced with chaining functions

        if(m_transType == SNDCHN_TT_ROT_MAN) {
            m_pivot.set(m_pathContainer.localToWorldPoint(ofVec2f(-0.35, 0.35)));  //local position
            m_pivMin = 0;
            m_pivMax = 90;

        }

        if(m_transType == SNDCHN_TT_TRANS_MAN) {

            //calculate vector
            //intersects shhould be used in a different method
            // vector<ofVec2f> intersects = m_pathContainer.getIntersects(m_transDir, m_posGlobalO);
            m_transDir.set(0,1); //adjust later
            m_transDir = m_pathContainer.localToWorldVec(m_transDir);

            m_transBounds.clear();
            m_transBounds.push_back(m_posGlobalC - m_transDir * 100);
            m_transBounds.push_back(m_posGlobalC + m_transDir * 100);


        }

        if(m_transType == SNDCHN_TT_TRANS_AUTO) {

            m_transDir = m_pathContainer.localToWorldVec(ofVec2f(0,-100));

            m_transBounds.clear();
            m_transBounds = m_pathContainer.getIntersects(m_transDir, m_posGlobalO);

            ofPoint p = m_pathContainer.getPos();

            ofVec2f t_vecs[2];

            for(int i = 0 ; i < 2 ; i++){
                t_vecs[i] = m_posGlobalO - m_transBounds[i];
                t_vecs[i] = t_vecs[i].getLimited(t_vecs[i].length() - 35);
                m_transBounds[i] = p + t_vecs[i];
            }

        }


    }




}

void button::drag(ofVec2f t_mouse) {


    m_isPointInside = t_mouse.distance(m_posGlobalC) <= m_radius;
    if(!m_isSelected)return;


    if(m_transType == SNDCHN_TT_ROT_MAN) {

        ofVec2f v = m_pivot - t_mouse;
        ofVec2f v2 = m_pivot - m_posGlobalC;

        float angle = -v.angle(v2);
        angle = min(1.0f, max(angle, -1.0f));

        ofVec2f p(m_posGlobalC);
        p.rotate(angle, m_pivot);

        float ang_moved = ofVec2f(m_pivot - m_posGlobalO).angle(ofVec2f(m_pivot - p));

        if(ang_moved <= m_pivMax && ang_moved >= m_pivMin) {

            m_posGlobalC.set(p);
            m_pathContainer.rotateMan(angle ,m_pivot); //no point to gearing for this . (?)

        }

    }

    if(m_transType == SNDCHN_TT_TRANS_MAN) {

        ofVec2f v = m_transBounds[1] - m_transBounds[0];
        ofVec2f p;

        float k = (v.y * (t_mouse.x - m_transBounds[0].x) - v.x * (t_mouse.y - m_transBounds[0].y)) / (pow(v.y, 2) + pow(v.x, 2));

        p.x = t_mouse.x - k * v.y;
        p.y = t_mouse.y + k * v.x;

        //constrain the proportions
        p.x = max(p.x, min(m_transBounds[0].x, m_transBounds[1].x));
        p.x = min(p.x, max(m_transBounds[0].x, m_transBounds[1].x));
        p.y = max(p.y, min(m_transBounds[0].y, m_transBounds[1].y));
        p.y = min(p.y, max(m_transBounds[0].y, m_transBounds[1].y));

        ofVec2f t(p - m_posGlobalC);
        m_pathContainer.translateMan(t.getNormalized() * t.length() * 1.0); // can be a gearing ratio here but will need boundary checking
        m_posGlobalC.set(p);

    }





}

void button::release() {

    if(m_isSelected) {
        m_posGlobalO = m_posGlobalC; // store last position b4 next transform
        m_pathContainer.updateO();
    }

    m_isSelected = false;
    m_isPointInside = false;

}

//helper functions

void button::calcIsMoving(){



    m_hist.push_back(ofVec2f(m_posGlobalC.x, m_posGlobalC.y));

    if(m_hist.size() > m_histSize)m_hist.erase(m_hist.begin());

    if(m_hist.size()  > 2) {
        float t_dist = 0;
        for(int i = 0; i < m_hist.size() - 1; i ++)t_dist += m_hist[i].distance(m_hist[i + 1]);
        m_isMoving = t_dist > 1;
    }

}

void button::handleOSC(){

    //send on moving mode ... other modes to follow

    ofxOscMessage m;

    if(m_transType < 3){

        if(m_isMoving && ! m_isSoundOn) {
            m.setAddress("/startS");
            p_sender->sendMessage(m);
            m_isSoundOn = true;
        } else if(!m_isMoving && m_isSoundOn) {
            m.setAddress("/stopS");
            p_sender->sendMessage(m);
            m_isSoundOn = false;
        } else if(m_isSoundOn) {
            m.setAddress("/updateS"); //TODO add params here
            p_sender->sendMessage(m);
        }

    }else{

        if(m_isSelected && ! m_isSoundOn) {
            m.setAddress("/startS");
            p_sender->sendMessage(m);
            m_isSoundOn = true;
        } else if(!m_isSelected && m_isSoundOn) {
            m.setAddress("/stopS");
            p_sender->sendMessage(m);
            m_isSoundOn = false;
        } else if(m_isSoundOn) {
            m.setAddress("/updateS"); //TODO add params here
            p_sender->sendMessage(m);
        }


    }


}


ofVec2f button::getModMouse() {
    return m_posGlobalC;
}

bool button::getIsSelected() {
    return m_isSelected;
}

bool button::getIsPointInside() {
    return m_isPointInside;
}


void button::setOSCSender( ofPtr<ofxOscSender> o) {
    p_sender = o;
}


button::~button() {
    //dtor
}
