#include "button.h"

button::button() {

    ofRectangle r = m_pathContainer.getDims();

    //ctor
    m_isSelected = false;
    m_radius = 15;
    m_posO.set(r.width * 0.35,-r.height * 0, 0);
    m_posC.set(m_posO);

    m_histSize = 10;

    m_isSoundOn = false;
    m_isMoving = false;

   // m_transType = transType(1);
    m_transType = SNDCHN_TT_TRANS_AUTO;

    m_inputMapper = shared_ptr<inputMapper>(new holdInput());

}

void button::update() {


    m_timeAcc += ofGetLastFrameTime();
    if(m_timeAcc < 1.0/120.0)return; // lock to prevent too fast frame updates
    m_timeAcc = 0;

    if(!m_inputMapper->getIsUserInput() && m_inputMapper->getIsActive())m_inputMapper->update();

    calcIsMoving();
    handleOSC();



    if(m_isSelected) {

        if(m_transType == SNDCHN_TT_ROT_AUTO) {

        }


        if(m_transType == SNDCHN_TT_TRANS_AUTO) {

            m_pathContainer.translateMan( valToVec(m_inputMapper->getVal(), m_pathContainer.getBounds(), m_posO));

        }

    }

}



void button::draw() {

    ofPushStyle();
    ofSetColor(0);
    (m_isSelected)? ofFill(): ofNoFill();
    ofCircle(m_posC, m_radius);
    ofPopStyle();
    m_pathContainer.draw();

    //debug drawing

    //ofCircle(m_pivot,5);
    /*if(m_transBounds.size() > 0){
        ofCircle(m_transBounds[0], 5);
        ofCircle(m_transBounds[1], 10);
    }


    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRect(m_pathContainer.getPos(), 6,6);
    ofPopStyle();*/


}

//mouse interaction

void button::press(ofVec2f t_mouse) {

    //mouse coordinates already translated in testApp
    m_isPointInside = t_mouse.distance(m_posC) <= m_radius;
    m_isSelected = m_isPointInside;

    if(m_isSelected) {

        //m_transType = transType(((int)m_transType + 1)%3); //ultimately this will be replaced with chaining functions

        if(m_transType == SNDCHN_TT_ROT_MAN) {

            m_pivot.set(m_pathContainer.localToWorldPoint(ofVec2f(-0.35, 0.35)));  //local position
            m_pivRange = 180;
            m_pivStart = 0.5;

        }

        if(m_transType == SNDCHN_TT_TRANS_MAN) {

            ofVec2f v(0,1); //adjust later
            v = m_pathContainer.localToWorldVec(v);

            m_transBounds.clear();
            m_transBounds.push_back(m_posC - v * 100);
            m_transBounds.push_back(m_posC + v * 100);


        }

        if(m_transType == SNDCHN_TT_TRANS_AUTO) {

            //set vec bounds from container
            ofVec2f v = m_pathContainer.localToWorldVec(ofVec2f(0,-1));
            m_pathContainer.setBounds(m_posO, v, 35);

        }



        //needs an if statement for input types here
        if(m_inputMapper->getType() == "arcInput"){

            shared_ptr<arcInput> t_arc(static_pointer_cast <arcInput>(m_inputMapper)); //downcast it
            t_arc->setRangeDegrees(m_pivRange);
            t_arc->setPivot(m_pivot);
            t_arc->setPosO(m_posC);
            t_arc->start(m_pivStart);

        }

        if(m_inputMapper->getType() == "vecInput"){

            shared_ptr<vecInput> t_vec(static_pointer_cast <vecInput>(m_inputMapper)); //downcast it
            t_vec->setBounds(m_transBounds[0], m_transBounds[1]);
            t_vec->setPosO(m_posC);
            t_vec->start();

        }

        if(m_inputMapper->getType() == "holdInput"){

            shared_ptr<holdInput> t_hold(static_pointer_cast <holdInput>(m_inputMapper)); //downcast it
            t_hold->setTime(5.0);
            t_hold->setIsPingPong(true);
            vector<ofVec2f> t_vecs = m_pathContainer.getBounds();
            float sp = ofVec2f(m_posO - t_vecs[0]).length()/ofVec2f(t_vecs[1] - t_vecs[0]).length();
            t_hold->start(sp);

        }


    }




}

void button::drag(ofVec2f t_mouse) {


    m_isPointInside = t_mouse.distance(m_posC) <= m_radius;

    if(!m_isSelected)return;

    if(m_inputMapper->getIsUserInput())m_inputMapper->update(t_mouse);


    if(m_transType == SNDCHN_TT_ROT_MAN){

        float ang = valToRot(m_inputMapper->getVal(), m_pivRange, m_pivStart);
        m_posC = m_posO.getRotated(ang, m_pivot, ofVec3f(0,0,1));
        m_pathContainer.rotateMan(ang, m_pivot);

    }

    if(m_transType == SNDCHN_TT_TRANS_MAN){

        m_posC = m_posO + valToVec(m_inputMapper->getVal(), m_transBounds, m_posO);
        m_pathContainer.translateMan( valToVec(m_inputMapper->getVal(), m_transBounds, m_posO)); //bounds should belong to pathContainer ?

    }




}

void button::release() {

    if(m_isSelected) {
        m_inputMapper->stop();
        m_posO = m_posC; // store last position b4 next transform
        m_pathContainer.updateO();
    }

    m_isSelected = false;
    m_isPointInside = false;

}

//helper functions

ofVec2f button::valToVec(float val, vector<ofVec2f> bnds, ofVec2f i_pos){

    /* convert value from an input function to
    a vector based on bounds and initial position */

    ofVec2f v = bnds[1] - bnds[0];
    ofVec2f v2 = i_pos - bnds[0];
    return val * v - v2;

}

float button::valToRot(float val, float rng, float i_rot){
    return(val - i_rot) * rng;
}


void button::calcIsMoving(){

    m_hist.push_back(ofVec2f(m_posC.x, m_posC.y));

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
    return m_posC;
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
