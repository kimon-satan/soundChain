#include "button.h"

button::button(shared_ptr<pathContainer> p) {

    m_pathContainer = p;
    ofRectangle r = m_pathContainer->getDims();

    //ctor
    m_isSelected = false;
    m_isPointInside = false;
    m_radius = 15;
    m_posO.set(r.width * 0.35,-r.height * 0, 0);
    m_posC.set(m_posO);

    m_histSize = 10;

    m_isSoundOn = false;
    m_isMoving = false;

    m_tData.reset(); //= shared_ptr<baseTData>(new transTData());
    m_inputMapper = shared_ptr<inputMapper>(new holdInput());

    c_input = 0;

}

void button::switchInput(){

    c_input = (c_input + 1)%3;

    if(c_input == 0){

        m_tData.reset(); //= shared_ptr<baseTData>(new transTData());
        m_inputMapper = shared_ptr<inputMapper>(new holdInput());

    }

    if(c_input == 1){

        m_tData = shared_ptr<baseTData>(new transTData());
        m_inputMapper = shared_ptr<inputMapper>(new vecInput());


    }

    if(c_input == 2){

        m_tData = shared_ptr<baseTData>(new pivotTData());
        m_inputMapper = shared_ptr<inputMapper>(new arcInput());

    }


}

void button::update() {

    // lock to prevent too fast frame updates
    // add interpolation for drawing later
    m_timeAcc += ofGetLastFrameTime();
    if(m_timeAcc < 1.0/120.0)return;
    m_timeAcc = 0;

    if(!m_inputMapper->getIsUserInput() && m_inputMapper->getIsActive())m_inputMapper->update();

    calcIsMoving();
    handleOSC();

    if(m_isSelected) {

        if(!m_inputMapper->getIsUserInput()){
            m_inputMapper->update(ofVec2f(0,0));
            m_pathContainer->update(m_inputMapper->getVal());
        }


    }

}



void button::draw() {

    ofPushStyle();
    ofSetColor(0);
    (m_isSelected)? ofFill(): ofNoFill();
    ofCircle(m_posC, m_radius);
    ofPopStyle();
    m_pathContainer->draw();

    //debug drawing

    //ofCircle(m_pivot,5);
    /*if(m_transBounds.size() > 0){
        ofCircle(m_transBounds[0], 5);
        ofCircle(m_transBounds[1], 10);
    }


    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRect(m_pathContainer->getPos(), 6,6);
    ofPopStyle();*/


}

//mouse interaction

void button::press(ofVec2f t_mouse) {

    //mouse coordinates already translated in testApp
    m_isPointInside = t_mouse.distance(m_posC) <= m_radius;
    m_isSelected = m_isPointInside;

    if(m_isSelected) {

        m_pathContainer->start(m_posO);

        //needs an if statement for input types here
        if(m_inputMapper->getType() == "arcInput"){

            shared_ptr<pivotTData> t_piv(static_pointer_cast <pivotTData>(m_tData));

            t_piv->range = 180;
            t_piv->start = 0.5;
            t_piv->pivotLocal.set(ofVec2f(-0.35, 0.35));  //local position

            if(t_piv->isLocal)
                t_piv->pivotGlobal.set(m_pathContainer->localToWorldPoint(t_piv->pivotLocal));
            else
                t_piv->pivotGlobal.set(t_piv->pivotLocal);

            shared_ptr<arcInput> t_arc(static_pointer_cast <arcInput>(m_inputMapper)); //downcast it

            t_arc->setPosO(m_posC);

            t_arc->setRangeDegrees(t_piv->range); //could set via data structure
            t_arc->setPivot(t_piv->pivotGlobal);
            t_arc->start(t_piv->start);



        }

        if(m_inputMapper->getType() == "vecInput"){

            shared_ptr<transTData> t_trans(static_pointer_cast <transTData>(m_tData));

            t_trans->dirLocal.set(0,1);
            t_trans->isLocal = true;

            if(t_trans->isLocal)
                t_trans->dirGlobal.set(m_pathContainer->localToWorldVec(t_trans->dirLocal));
            else
                t_trans->dirGlobal.set(t_trans->dirLocal);

            t_trans->bounds.clear();
            t_trans->bounds.push_back(m_posO - t_trans->dirGlobal * 100);
            t_trans->bounds.push_back(m_posO + t_trans->dirGlobal * 100);

            shared_ptr<vecInput> t_vec(static_pointer_cast <vecInput>(m_inputMapper)); //downcast it
            t_vec->setBounds(t_trans->bounds[0], t_trans->bounds[1]);
            t_vec->setPosO(m_posC);
            t_vec->start();

        }

        if(m_inputMapper->getType() == "holdInput"){

            shared_ptr<holdInput> t_hold(static_pointer_cast <holdInput>(m_inputMapper)); //downcast it
            t_hold->setTime(5.0);
            t_hold->setIsPingPong(true);

            t_hold->start(m_pathContainer->getStartVal());

        }

    }




}

void button::drag(ofVec2f t_mouse) {


    m_isPointInside = t_mouse.distance(m_posC) <= m_radius;

    if(!m_isSelected)return;

    if(m_inputMapper->getIsUserInput()){
        m_inputMapper->update(t_mouse);
        m_pathContainer->update(m_inputMapper->getVal());
    }


    if(m_inputMapper->getType() == "arcInput"){

        shared_ptr<pivotTData> t_piv(static_pointer_cast <pivotTData>(m_tData));
        float ang = utils::valToRot(m_inputMapper->getVal(), t_piv->range, t_piv->start);
        m_posC = m_posO.getRotated(ang, t_piv->pivotGlobal, ofVec3f(0,0,1));

    }

    if(m_inputMapper->getType() == "vecInput"){

        shared_ptr<transTData> t_trans(static_pointer_cast <transTData>(m_tData));
        m_posC = m_posO + utils::valToVec(m_inputMapper->getVal(), t_trans->bounds, m_posO);

    }




}

void button::release() {

    if(m_isSelected) {
        m_inputMapper->stop();
        m_posO = m_posC; // store last position b4 next transform
        m_pathContainer->updateO();
    }

    m_isSelected = false;
    m_isPointInside = false;

}

//helper functions




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

   /* if(m_transType < 3){

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


    } */


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
