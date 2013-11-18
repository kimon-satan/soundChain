#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {

    ofSetCircleResolution(100);
    ofHideCursor();

    m_sender = ofPtr<ofxOscSender>(new ofxOscSender());
    m_sender->setup("localhost", 57120);

    ofxOscMessage m;
    m.setAddress("/init");
    m_sender->sendMessage(m);

    m_pathContainer = shared_ptr<pathContainer>(new pathContainer());
    m_button = shared_ptr<button>(new button(m_pathContainer));
    m_button->setOSCSender(m_sender);


    butType = 0;
    pathType = 0;

    modifyPath();
    modifyButton();



}


void testApp::modifyButton() {

    if(butType == 0) {

        shared_ptr<transTData> t_trans = shared_ptr<transTData>(new transTData());
        shared_ptr<inputMapper> t_inputMapper = shared_ptr<inputMapper>(new vecInput());

        t_trans->dirLocal.set(1, 0);
        t_trans->isLocal = true;
        t_trans->isBoundsFromPath = true;
        t_trans->isPixelBuf = true;
        t_trans->pixelBuf = 25;
        //t_trans->range = 100;
        //t_trans->start = 0.5;

        m_button->setInput(t_inputMapper);
        m_button->setTransform(t_trans);

        m_button->reset();

    }

    if(butType == 1) {

        shared_ptr<pivotTData> t_piv = shared_ptr<pivotTData>(new pivotTData());
        shared_ptr<inputMapper> t_inputMapper = shared_ptr<inputMapper>(new arcInput());

        t_piv->range = 180;
        t_piv->start = 0.5;
        t_piv->pivotLocal.set(ofVec2f(0.0,0.0));  //local position
        t_piv->isLocal = true;

        m_button->setInput(t_inputMapper);
        m_button->setTransform(t_piv);

        m_button->reset();


    }

    if(butType == 2) {

        shared_ptr<baseTData> t_bd;
        shared_ptr<inputMapper> t_inputMapper = shared_ptr<inputMapper>(new holdInput());

        m_button->setInput(t_inputMapper);
        m_button->setTransform(t_bd);
        m_button->reset();

    }

}

void testApp::modifyPath() {

    m_pathContainer->updateO();

    if(pathType == 0){

        shared_ptr<transTData> t_trans = shared_ptr<transTData>(new transTData());
        t_trans->dirLocal.set(0,1);
        t_trans->isBoundsFromPath = false;
        t_trans->isBoundsFromButton = false;
        t_trans->isLocal = true;
        t_trans->isPixelBuf = true;
        t_trans->pixelBuf = 25;
        t_trans->range = 100;
        t_trans->start = 0.5;

        m_pathContainer->setTransform(t_trans);

    }

    if(pathType == 1){

        shared_ptr<pivotTData> t_piv = shared_ptr<pivotTData>(new pivotTData());

        t_piv->range = 180;
        t_piv->start = 0.5;
        t_piv->pivotLocal.set(ofVec2f(-0.35, 0.35));  //local position

        m_pathContainer->setTransform(t_piv);
    }

    modifyButton();

}


//--------------------------------------------------------------
void testApp::update() {

    m_worldMouse.set(mouseX - ofGetWidth()/2, -(mouseY - ofGetHeight()/2));
    m_button->update();
    m_drawMouse = (m_button->getIsSelected()) ? m_button->getModMouse() : m_worldMouse;


}

//--------------------------------------------------------------
void testApp::draw() {

    ofBackground(255);
    ofSetColor(0);
    ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(),2),20,20);

    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0); //camera style coordinates incase used later
    ofScale(1,-1,1);

    m_button->draw();

    //draw mouse pointer
    m_button->getIsPointInside() ? ofSetColor(255) : ofSetColor(0);
    ofNoFill();

    ofLine(m_worldMouse.x - 10, m_worldMouse.y, m_worldMouse.x + 10, m_worldMouse.y);
    ofLine(m_worldMouse.x , m_worldMouse.y - 10, m_worldMouse.x , m_worldMouse.y + 10);


    ofPopMatrix();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {

    if(key == 'a') {
        butType = (butType + 1)%3;
        modifyButton();
    }

    if(key == 's') {
        pathType = (pathType + 1)%2;
        modifyPath();
    }



}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {

    m_button->drag(m_worldMouse);

}


//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {

    if(button == 0)m_button->press(m_worldMouse);

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {

    if(button == 0)m_button->release();

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
