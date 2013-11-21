#include "handle.h"

handle::handle() {

    //ctor
    m_isSelected = false;
    m_isPointInside = false;
    m_radius = 15;
    m_posO.set(0,0,0);
    m_posC.set(m_posO);
    m_rotO = 0;

    m_histSize = 10;

    m_isSoundOn = false;
    m_isMoving = false;

}


void handle::update() {

    // lock to prevent too fast frame updates
    // add interpolation for drawing later
    m_timeAcc += ofGetLastFrameTime(); //...probably in testApp
    if(m_timeAcc < 1.0/120.0)return;
    m_timeAcc = 0;

    if(m_isSelected) {
        if(m_inputMapper) {
            if(!m_inputMapper->getIsUserInput() && m_inputMapper->getIsActive())m_inputMapper->update();
        }
    }


    calcIsMoving();
    handleOSC();

}



void handle::draw() {

    ofPushStyle();
    ofSetColor(0);
    (m_isSelected)? ofFill(): ofNoFill();
    ofCircle(m_posC, m_radius);
    ofPopStyle();

    ofSetColor(0);
    vector <shared_ptr <handle> > :: iterator it = m_children.begin();

    while(it != m_children.end()) {
        ofLine(m_posC, (*it)->getPosC()); //could take radius into account later
        ++it;
    }

    //debug drawing

    /*if(m_inputMapper) {

        if(m_inputMapper->getType() == "arcInput") {

            shared_ptr<arcInput> ai(static_pointer_cast<arcInput>(m_inputMapper));

            ofCircle(ai->getPivot(), 5);
            ofFill();
            ofSetColor(255,0,0);

            ofCircle(ai->getPosC(),5);
            ofVec2f p( 0, 20);
            ofLine(ai->getPivot() , ai->getPivot() +  p.getRotated(m_rotC));
            ofLine(ai->getPivot() , m_posC);
        }

        if(m_inputMapper->getType()  == "vecInput") {

            shared_ptr<vecInput> vi(static_pointer_cast<vecInput>(m_inputMapper));
            for(int i = 0; i < 2; i++)ofCircle(vi->getBounds(i), 5);

        }

    }*/




}



//mouse interaction

shared_ptr<handle> handle::press(ofVec2f t_mouse) {

    //mouse coordinates already translated in testApp
    shared_ptr<handle> ptr;

    m_isSelected = m_isPointInside;

    if(m_isSelected) {

        if(!m_parent && m_children.size() == 0) {
            ptr = spawnHandle();
            m_inputMapper = shared_ptr<inputMapper>(new vecInput()); //need to think how parameters for this are set
            reset();
        }

        if(m_inputMapper) {
            reset();
            m_inputMapper->start();
        }

    }

    return ptr;

}

void handle::drag(ofVec2f t_mouse) {


    m_isPointInside = t_mouse.distance(m_posC) <= m_radius;

    if(!m_isSelected || !m_inputMapper)return;


    if(m_inputMapper->getIsUserInput()) {
        m_inputMapper->update(t_mouse);
        m_posC = m_inputMapper->getPosC();
    }

    if(m_inputMapper->getType() == "arcInput") {
        shared_ptr<arcInput> ai(static_pointer_cast<arcInput>(m_inputMapper));
        m_rotC = m_rotO + ai->getRotC();
    }



}

void handle::release() {

    m_isSelected = false;
    m_isPointInside = false;

    // deactivate input mapper
    if(m_inputMapper)m_inputMapper->stop();

}

//helper functions

void handle::reset() {

    m_posO = m_posC; // store last position b4 next transform
    m_rotO = m_rotC;

    if(!m_inputMapper)return;

    //needs an if statement for input types here
    if(m_inputMapper->getType() == "arcInput") {

        shared_ptr<arcInput> t_arc(static_pointer_cast <arcInput>(m_inputMapper)); //downcast it

        ofVec2f p;
        p.set(m_hook->getPosC());

        t_arc->setPosO(m_posO);
        t_arc->setBoundsDegrees(-90,90); //could be set in a variety of ways (a data structure will be needed)
        t_arc->setPivot(p);
        t_arc->reset();


    }

    if(m_inputMapper->getType() == "vecInput") {

        shared_ptr<vecInput> t_vec(static_pointer_cast <vecInput>(m_inputMapper));
        vector<ofVec2f> t_bounds;
        ofVec2f t_dir(m_posC - m_hook->getPosC());

        if(t_dir.length() > 0) {
            t_vec->setDirGlobal(t_dir.getNormalized());

            if(t_dir.length() < 100) { //this can become a parameter later
                t_bounds.push_back(m_posC);
            } else {
                t_bounds.push_back(m_posC - t_vec->getDirGlobal() * 100);
            }

            t_bounds.push_back(m_posC + t_vec->getDirGlobal() * 100);

        } else {
            t_vec->setDirGlobal(ofVec2f(0,1)); //something for this later which locks after intial decision (or use mouse to determine direction)
            t_bounds.push_back(m_posC );
            t_bounds.push_back(m_posC + t_vec->getDirGlobal() * 100);
        }


        t_vec->setBounds(t_bounds[0], t_bounds[1]);
        t_vec->setPosO(m_posC);
        t_vec->reset();

    }

    if(m_inputMapper->getType() == "holdInput") {

        shared_ptr<holdInput> t_hold(static_pointer_cast <holdInput>(m_inputMapper)); //downcast it
        t_hold->setTime(5.0);
        t_hold->setIsPingPong(true);
        t_hold->setBounds(-1, 0.05);

    }

}




void handle::calcIsMoving() {

    m_hist.push_back(ofVec2f(m_posC.x, m_posC.y));

    if(m_hist.size() > m_histSize)m_hist.erase(m_hist.begin());

    if(m_hist.size()  > 2) {
        float t_dist = 0;
        for(int i = 0; i < m_hist.size() - 1; i ++)t_dist += m_hist[i].distance(m_hist[i + 1]);
        m_isMoving = t_dist > 1;
    }

}

void handle::handleOSC() {

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



bool handle::getIsSelected() {
    return m_isSelected;
}

bool handle::getIsPointInside(ofVec2f t_mouse) {
    m_isPointInside = t_mouse.distance(m_posC) <= m_radius;
    return m_isPointInside;
}

void handle::setOSCSender( ofPtr<ofxOscSender> o) {
    p_sender = o;
}

void handle::setInput(shared_ptr <inputMapper> p ) {
    m_inputMapper = p;
}

shared_ptr <handle> handle::spawnHandle() {

    shared_ptr <handle> h = shared_ptr<handle>(new handle());
    h->setPos(m_posC);
    h->setRadius(5);
    m_hook = h;
    m_children.push_back(h);
    return h;
}

ofVec2f handle::getPosC() {
    return m_posC;
}

float handle::getRotC(){
    return m_rotC;
}

void handle::setPos(ofVec2f p) {
    m_posO.set(p);
    m_posC.set(p);
}

void handle::setRadius(float r) {
    m_radius = r;
}

void handle::setParent(shared_ptr <handle> t) {
    m_parent = t;
    m_hook = m_parent;
}
void handle::setHook(shared_ptr <handle> t) {
    m_hook = t;
}

void handle::setLevel(int i) {
    m_level = i;
}

int handle::getLevel() {
    return m_level;
}

handle::~handle() {
    //dtor
}
