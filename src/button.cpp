#include "button.h"

button::button(shared_ptr<pathContainer> p) {

    m_pathContainer = p;
    ofRectangle r = m_pathContainer->getDims();

    //ctor
    m_isSelected = false;
    m_isPointInside = false;
    m_radius = 15;
    m_posO.set(r.width * 0.35,-r.height * 0.35, 0);
    m_posC.set(m_posO);

    m_histSize = 10;

    m_isSoundOn = false;
    m_isMoving = false;




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

        if(!m_inputMapper->getIsUserInput()) {
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

    if(m_tData){

        if(m_tData->type == "pivot") {
            shared_ptr<pivotTData> t_piv(static_pointer_cast<pivotTData>(m_tData));
            ofCircle(t_piv->pivotGlobal, 5);
            ofLine(t_piv->pivotGlobal , m_posC);
        }

         if(m_tData->type == "translate") {
            shared_ptr<transTData> t_trans(static_pointer_cast<transTData>(m_tData));
            for(int i = 0; i < 2; i++)ofCircle(t_trans->bounds[i], 5);

        }

    }

    /*
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
    if(m_isSelected)m_inputMapper->start();


}

void button::drag(ofVec2f t_mouse) {


    m_isPointInside = t_mouse.distance(m_posC) <= m_radius;

    if(!m_isSelected)return;

    if(m_inputMapper->getIsUserInput()) {
        m_inputMapper->update(t_mouse);
        m_pathContainer->update(m_inputMapper->getVal());
    }


    if(m_inputMapper->getType() == "arcInput") {

        shared_ptr<pivotTData> t_piv(static_pointer_cast <pivotTData>(m_tData));
        float ang = utils::valToRot(m_inputMapper->getVal(), t_piv->range, t_piv->start);
        m_posC = m_posO.getRotated(ang, t_piv->pivotGlobal, ofVec3f(0,0,1));

    }

    if(m_inputMapper->getType() == "vecInput") {

        shared_ptr<transTData> t_trans(static_pointer_cast <transTData>(m_tData));
        m_posC = m_posO + utils::valToVec(m_inputMapper->getVal(), t_trans->bounds, m_posO);

    }




}

void button::release() {

    m_isSelected = false;
    m_isPointInside = false;

   // deactivate input mapper
   m_inputMapper->stop();

}

//helper functions

void button::reset() {

    m_posO = m_posC; // store last position b4 next transform
    m_pathContainer->updateO(); //likewise for the path

    //needs an if statement for input types here
    if(m_inputMapper->getType() == "arcInput") {

        shared_ptr<pivotTData> t_piv(static_pointer_cast<pivotTData>(m_tData));

        if(t_piv->isLocal)
            t_piv->pivotGlobal.set(m_pathContainer->localToWorldPoint(t_piv->pivotLocal));
        else
            t_piv->pivotGlobal.set(t_piv->pivotLocal);

        shared_ptr<arcInput> t_arc(static_pointer_cast <arcInput>(m_inputMapper)); //downcast it

        t_arc->setPosO(m_posO);

        t_arc->setRangeDegrees(t_piv->range); //could set via data structure
        t_arc->setPivot(t_piv->pivotGlobal);
        t_arc->setStartVal(t_piv->start);

        //control needs to own start_val
        m_pathContainer->start(m_posO, t_piv->start);

    }

    if(m_inputMapper->getType() == "vecInput") {

        shared_ptr<transTData> t_trans(static_pointer_cast <transTData>(m_tData));

        if(t_trans->isLocal)
            t_trans->dirGlobal.set(m_pathContainer->localToWorldVec(t_trans->dirLocal));
        else
            t_trans->dirGlobal.set(t_trans->dirLocal);

        if(t_trans->isBoundsFromPath){

            ofPolyline tp = m_pathContainer->getPolyA();

            if(t_trans->isPixelBuf){
                pathUtils::setBoundsFromPath(tp, t_trans->bounds ,m_posO, t_trans->dirGlobal, t_trans->pixelBuf);
            }else{
                pathUtils::setBoundsFromPath(tp, t_trans->bounds ,m_posO, t_trans->dirGlobal, t_trans->prop);
            }

            t_trans->start = ofVec2f(m_posO - t_trans->bounds[0]).length()/ofVec2f(t_trans->bounds[1] - t_trans->bounds[0]).length();
            m_pathContainer->start(m_posO, t_trans->start);

        }else{

            t_trans->bounds.clear();
            t_trans->bounds.push_back(m_posO - t_trans->dirGlobal * t_trans->range * t_trans->start);
            t_trans->bounds.push_back(m_posO + t_trans->dirGlobal * t_trans->range * (1 - t_trans->start));
            m_pathContainer->start(m_posO, t_trans->start);

        }

        shared_ptr<vecInput> t_vec(static_pointer_cast <vecInput>(m_inputMapper)); //downcast it
        t_vec->setBounds(t_trans->bounds[0], t_trans->bounds[1]);
        t_vec->setPosO(m_posC);

    }

    if(m_inputMapper->getType() == "holdInput") {

        m_pathContainer->start(m_posO); //not sure this should be here either
        shared_ptr<holdInput> t_hold(static_pointer_cast <holdInput>(m_inputMapper)); //downcast it
        t_hold->setTime(5.0);
        t_hold->setIsPingPong(true);
        t_hold->setStartVal(m_pathContainer->getStartVal()); // path container has start val put this somewhere else

    }

}




void button::calcIsMoving() {

    m_hist.push_back(ofVec2f(m_posC.x, m_posC.y));

    if(m_hist.size() > m_histSize)m_hist.erase(m_hist.begin());

    if(m_hist.size()  > 2) {
        float t_dist = 0;
        for(int i = 0; i < m_hist.size() - 1; i ++)t_dist += m_hist[i].distance(m_hist[i + 1]);
        m_isMoving = t_dist > 1;
    }

}

void button::handleOSC() {

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

void button::setInput(shared_ptr <inputMapper> p ) {
    m_inputMapper = p;
}

void button::setTransform(shared_ptr <baseTData> b) {
    m_tData = b;
}

ofVec2f button::getPos(){return m_posC;}

button::~button() {
    //dtor
}
