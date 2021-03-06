#include "handle.h"

handle::handle() {

    //ctor
    m_isActive = false;
    m_isSelected = false;

    m_radius = 15;
    m_posC.set(0,0,0);
    m_rotC = 0;

    m_histSize = 10;

    m_isSoundOn = false;
    m_isMoving = false;
    utils::genRandom(m_index, 10);

}


void handle::update() {



    if(m_isActive) {
        if(m_inputMapper) {
            if(!m_inputMapper->getIsUserInput() && m_inputMapper->getIsActive())m_inputMapper->update();
        }
    }


    calcIsMoving();
    handleOSC();

}



void handle::draw() {

    ofPushStyle();

    ofFill();

    if(m_isActive) {
        ofSetColor(0);
    } else if(m_isSelected) {
        ofSetColor(255,100,100);
    } else {
        ofSetColor(255);
    }

    ofCircle(m_posC, m_radius);
    ofNoFill();

    ofSetColor(0);
    ofCircle(m_posC, m_radius);

    ofPopStyle();


    //debug drawing

    ofSetColor(0);
    ofVec2f v(0,m_radius);
    v.rotate(m_rotC);
    ofLine(m_posC, m_posC + v);

    ofDrawBitmapString(ofToString(m_level,0),m_posC + ofVec2f(10,10));

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


void handle::drawSpines(){

    ofSetColor(0);
    vector <handleJoint> :: iterator it = m_children.begin();

    while(it != m_children.end()) {
        ofLine(m_posC, it->m_handle->getPosC()); //could take radius into account later
        ++it;
    }
}


//mouse interaction

shared_ptr<handle> handle::press(ofVec2f t_vec, int ha) {

    //mouse coordinates already translated in testApp
    shared_ptr<handle> ptr;

    m_isActive = true;

    if(!m_parent && m_children.size() == 0 || ha == HA_VEC_SPAWN) {

        ptr = spawnHandle();
        m_inputMapper = shared_ptr<inputMapper>(new vecInput()); //need to think how parameters for this are set
        shared_ptr<vecInput> t_vi(static_pointer_cast <vecInput>(m_inputMapper));

        if(t_vec.length() > 10){
            t_vi->setDirGlobal(t_vec.getNormalized());
        }else{
            t_vi->setDirGlobal(ofVec2f(0,1));
        }

    }

    if(m_inputMapper) {
        reset();
        m_inputMapper->start();
    }


    return ptr;

}

void handle::drag(ofVec2f t_mouse) {

    if(!m_isActive || !m_inputMapper)return;


    if(m_inputMapper->getIsUserInput()) {
        m_inputMapper->update(t_mouse);
        m_posC = m_inputMapper->getPosC();
    }

    if(m_inputMapper->getType() == "arcInput") {
        shared_ptr<arcInput> ai(static_pointer_cast<arcInput>(m_inputMapper));
        pivot(ai->getFrameRot(), ai->getPivot(), ofVec2f(0,0), "weld", m_index);
    }



}

void handle::release() {

    m_isActive = false;

    // deactivate input mapper
    if(m_inputMapper)m_inputMapper->stop();

}

//helper functions

void handle::reset() {

    if(!m_inputMapper)return;

    //needs an if statement for input types here
    if(m_inputMapper->getType() == "arcInput") {

        shared_ptr<arcInput> t_arc(static_pointer_cast <arcInput>(m_inputMapper)); //downcast it

        ofVec2f p;
        p.set(m_hook->getPosC());

        t_arc->setPosO(m_posC);
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
                t_bounds.push_back(m_hook->getPosC());
            } else {
                t_bounds.push_back(m_posC - t_vec->getDirGlobal() * 100);
            }

            t_bounds.push_back(m_posC + t_vec->getDirGlobal() * 100);

        } else {

            t_bounds.push_back(m_posC);
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




void handle::pivot(float f, ofVec2f p, ofVec2f t, string jType, string actor){

    ofVec2f np(m_posC);

    if(jType == "weld"){

        np = m_posC.getRotated(f, p, ofVec3f(0,0,1));
        t.set(np - m_posC);
        m_posC.set(np);
        m_rotC += f;

    }else if(jType == "pivot"){
        if(m_posC != p){
            m_posC += t;
        }
    }

    //recursive function call on children
    vector <handleJoint>::iterator it = m_children.begin();

    while(it != m_children.end()){
        if(it->m_handle->getIndex() == actor){++it; continue;} //actor is to prevent double calls
        if(it->m_type == "free"){++it; continue;}
        if(jType == "weld" &&  it->m_type == "weld"){
            it->m_handle->pivot(f, p, t, "weld", m_index);
        }else if(jType == "pivot" || it->m_type == "pivot"){
            if(m_posC != p)it->m_handle->pivot(f, p, t, "pivot", m_index);
        }
        ++it;
    }

    //pivot the parent
    if(m_parent){
        if(actor != m_parent->getIndex()){

            handleJoint hj = m_parent->getChildJoint(m_index);

            if(jType == "weld" &&  hj.m_type == "weld"){
                m_parent->pivot(f, p, t, "weld", m_index);
            }else if(jType == "pivot" || hj.m_type == "pivot"){
                if(m_posC != p)m_parent->pivot(f, p, t, "pivot", m_index);
            }

        }
    }

}

shared_ptr <handle> handle::spawnHandle() {

    shared_ptr <handle> h = shared_ptr<handle>(new handle());
    h->setPos(m_posC);
    h->setRadius(5);
    h->setLevel(m_level + 1);

    shared_ptr<inputMapper> t_inputMapper = shared_ptr<inputMapper>(new arcInput());
    h->setInput(t_inputMapper);

    handleJoint hj;
    hj.m_handle = h;
    hj.m_rot = 0;
    hj.m_trans.set(0,0);
    hj.m_type = "weld";

    m_children.push_back(hj);
    m_hook = hj.m_handle;

    return h;
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

         if(m_isActive && ! m_isSoundOn) {
             m.setAddress("/startS");
             p_sender->sendMessage(m);
             m_isSoundOn = true;
         } else if(!m_isActive && m_isSoundOn) {
             m.setAddress("/stopS");
             p_sender->sendMessage(m);
             m_isSoundOn = false;
         } else if(m_isSoundOn) {
             m.setAddress("/updateS"); //TODO add params here
             p_sender->sendMessage(m);
         }


     } */


}

handleJoint handle::getChildJoint(string index){

    for(int i = 0; i < m_children.size(); i++){
        if(m_children[i].m_handle->getIndex() == index)return m_children[i];
    }

}

void handle::setChildJoint(string index, string jt){

    for(int i = 0; i < m_children.size(); i++){
        if(m_children[i].m_handle->getIndex() == index){
            m_children[i].m_type = jt;
            break;
        }
    }

}

void handle::setParent(shared_ptr <handle> t) {
    m_parent = t;
}

void handle::setHook(shared_ptr <handle> t) {
    m_hook = t;
}

bool handle::getIsActive(){
    return m_isActive;
}

void handle::setIsSelected(bool b) {
    m_isSelected = b;
}

bool handle::getIsSelected() {
    return m_isSelected;
}

bool handle::getIsPointInside(ofVec2f t_mouse) {
    return t_mouse.distance(m_posC) <= m_radius;
}

void handle::setOSCSender( ofPtr<ofxOscSender> o) {
    p_sender = o;
}

void handle::setInput(shared_ptr <inputMapper> p ) {
    m_inputMapper = p;
}

ofVec2f handle::getPosC() {
    return m_posC;
}

float handle::getRotC() {
    return m_rotC;
}

void handle::setPos(ofVec2f p) {
    m_posC.set(p);
}

void handle::setRadius(float r) {
    m_radius = r;
}



void handle::setLevel(int i) {
    m_level = i;
}

int handle::getLevel() {
    return m_level;
}

string handle::getIndex(){
    return m_index;
}

string handle::getInputType(){
    return m_inputMapper->getType();
}

string handle::getJointType(){
    //parent has joints = not a great design
    if(m_parent){
        handleJoint hj = m_parent->getChildJoint(m_index);
        return hj.m_type;
    }else{
        return "";
    }
}

void handle::setJointType(string jt){

    if(m_parent){
        m_parent->setChildJoint(m_index, jt);
    }
}

handle::~handle() {
    //dtor
}
