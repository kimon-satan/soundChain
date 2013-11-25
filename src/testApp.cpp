#include "testApp.h"


//globals

bool sortByLevel(shared_ptr<handle> a, shared_ptr<handle> b) {
    return (a->getLevel() < b->getLevel());
}

//--------------------------------------------------------------
void testApp::setup() {

    ofSetCircleResolution(100);
    ofHideCursor();

    m_sender = ofPtr<ofxOscSender>(new ofxOscSender());
    m_sender->setup("localhost", 57120);

    ofxOscMessage m;
    m.setAddress("/init");
    m_sender->sendMessage(m);

    m_vessel = shared_ptr<vessel>(new vessel());

    m_handle = shared_ptr<handle>(new handle());
    m_handle->setOSCSender(m_sender);
    m_handle->setLevel(0);
    m_allHandles.push_back(m_handle);
    initMoveJoint(m_handle);

    m_handle.reset();

    butType = 0;



}


void testApp::modifyhandle(shared_ptr <handle> h ) {

    if(h->getInputType() == "arcInput") {

        shared_ptr<inputMapper> t_inputMapper = shared_ptr<inputMapper>(new vecInput());
        h->setInput(t_inputMapper);
        h->reset();

    }else{

        shared_ptr<inputMapper> t_inputMapper = shared_ptr<inputMapper>(new arcInput());
        h->setInput(t_inputMapper);
        h->reset();

    }

    /*if(butType == 2) {

        shared_ptr<inputMapper> t_inputMapper = shared_ptr<inputMapper>(new holdInput());
        h->setInput(t_inputMapper);
        h->reset();

    }*/

}

void testApp::modifyJoint(shared_ptr <handle> h){



   if(h->getJointType() == "pivot"){
        h->setJointType("weld");
    }else if(h->getJointType() == "weld"){
        h->setJointType("pivot");
    }

}


void testApp::initMoveJoint(shared_ptr <handle> h) {

    m_moveJoint.m_handle = h;
    m_moveJoint.m_type = "weld";
    m_moveJoint.m_rot =  m_vessel->getRot() - h->getRotC();
    m_moveJoint.m_trans =  m_vessel->getPos() - h->getPosC();

}

//--------------------------------------------------------------
void testApp::update() {

    //non regulated actions-------------------------------------------------------//

    m_worldMouse.set(mouseX - ofGetWidth()/2, -(mouseY - ofGetHeight()/2));

    if(m_isMouseDown && !m_isMouseActive){
        m_isMouseActive = (ofGetSystemTime()  - m_downCount > 200);
        if(m_isMouseActive)beginAction();
    }

    // lock to prevent too fast frame updates
    // add interpolation for drawing later
    m_timeAcc += ofGetLastFrameTime(); //...probably in testApp
    if(m_timeAcc < 1.0/120.0)return;
    m_timeAcc = 0;


    //regulated below here ------------------------------------------------------------//

    vector<shared_ptr<handle> >::iterator it = m_allHandles.begin();

    while(it != m_allHandles.end()) {
        (*it)->update();
        it++;
    }

    if(m_moveJoint.m_handle) {

        if(m_moveJoint.m_type == "weld") { // simpler if it just uses frameRot

            m_vessel->setPos( m_moveJoint.m_handle->getPosC() - m_moveJoint.m_trans);
            m_vessel->setRot( m_moveJoint.m_handle->getRotC() - m_moveJoint.m_rot);
        }

        if(m_moveJoint.m_type == "pivot") {

            m_vessel->setPos( m_moveJoint.m_handle->getPosC() - m_moveJoint.m_trans);

        }
    }


}


void testApp::beginAction(){

    int ha = (m_button == 0) ? HA_OTHER : HA_VEC_SPAWN;

    if(m_handle) {
        shared_ptr<handle> ptr = m_handle->press(m_dragVec, ha);
        if(ptr) {
            m_allHandles.push_back(ptr);
            ptr->setParent(m_handle);
            ptr->setHook(m_handle);
            sort(m_allHandles.begin(), m_allHandles.end(), sortByLevel );
        }
    }


}

//--------------------------------------------------------------
void testApp::draw() {

    ofBackground(255);
    ofSetColor(0);
    ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(),2),20,20);

    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0); //camera style coordinates incase used later
    ofScale(1,-1,1);

    m_vessel->draw();
    vector<shared_ptr<handle> >::iterator it = m_allHandles.end();

    while(it != m_allHandles.begin()) {
        it--;
        (*it)->drawSpines();
    }

    it = m_allHandles.end();

    while(it != m_allHandles.begin()) {
        it--;
        (*it)->draw();
    }

    //draw mouse pointer
    if(m_handle) {
        (m_handle->getIsActive() && m_handle->getIsPointInside(m_worldMouse)) ? ofSetColor(255) : ofSetColor(0);
    } else {
        ofSetColor(0);
    }

    ofNoFill();

    ofLine(m_worldMouse.x - 10, m_worldMouse.y, m_worldMouse.x + 10, m_worldMouse.y);
    ofLine(m_worldMouse.x , m_worldMouse.y - 10, m_worldMouse.x , m_worldMouse.y + 10);


    ofPopMatrix();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {

    if(key == 'a') {
        if(m_handle)modifyhandle(m_handle);
    }

    if(key == 's'){
        if(m_handle)modifyJoint(m_handle);
    }



}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {

    m_handle.reset();
    vector<shared_ptr<handle> >::iterator it = m_allHandles.begin();

    while(it != m_allHandles.end()) {
        (*it)->setIsSelected(false);
        it++;
    }

    it = m_allHandles.begin();

    while(it != m_allHandles.end() && !m_handle) {
        if((*it)->getIsPointInside(m_worldMouse)) {
            m_handle = *it;
            m_handle->setIsSelected(true);
        }
        it++;
    }

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {

    m_dragVec = m_worldMouse - m_pressPos;

    if(m_isMouseActive && m_handle){

        m_handle->drag(m_worldMouse);

    }

}


//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {

    m_downCount = ofGetSystemTime();
    m_button = button;
    m_isMouseDown = true;
    m_pressPos.set(m_worldMouse);



}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {

    m_downCount = 0;
    m_isMouseDown = false;
    m_isMouseActive = false;

    if(m_handle)m_handle->release();


}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo) {

}

void testApp::exit() {

    ofxOscMessage m;
    m.setAddress("/exit");
    m_sender->sendMessage(m);

}
