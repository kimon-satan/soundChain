#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofSetCircleResolution(100);
    ofHideCursor();

    m_sender = ofPtr<ofxOscSender>(new ofxOscSender());
    m_sender->setup("localhost", 57120);

    ofxOscMessage m;
    m.setAddress("/init");
    m_sender->sendMessage(m);

    m_button.setOSCSender(m_sender);

}

//--------------------------------------------------------------
void testApp::update(){

    m_worldMouse.set(mouseX - ofGetWidth()/2, -(mouseY - ofGetHeight()/2));
    m_button.update();
    m_drawMouse = (m_button.getIsSelected()) ? m_button.getModMouse() : m_worldMouse;

}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackground(255);
    ofSetColor(0);
    ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(),2),20,20);

    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0); //camera style coordinates incase used later
    ofScale(1,-1,1);

        m_button.draw();

        //draw mouse pointer
        m_button.getIsSelected() ? ofSetColor(255) : ofSetColor(0);
        ofNoFill();
        ofLine(m_drawMouse.x - 10, m_drawMouse.y, m_drawMouse.x + 10, m_drawMouse.y);
        ofLine(m_drawMouse.x , m_drawMouse.y - 10, m_drawMouse.x , m_drawMouse.y + 10);


    ofPopMatrix();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    m_button.drag(m_worldMouse);

}


//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    m_button.press(m_worldMouse);

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

    m_button.release();

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

void testApp::exit(){

    ofxOscMessage m;
    m.setAddress("/exit");
    m_sender->sendMessage(m);

}
