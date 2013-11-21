#include "testApp.h"


//globals

bool sortByLevel(shared_ptr<handle> a, shared_ptr<handle> b){
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
    m_moveJoint.m_handle = m_handle;
    m_moveJoint.m_type = "weld";

    m_handle.reset();

    butType = 0;
    pathType = 0;

    //modifyPath();
    //modifyhandle(m_handle);



}


void testApp::modifyhandle(shared_ptr <handle> h ) {

    if(butType == 0) {

        shared_ptr<inputMapper> t_inputMapper = shared_ptr<inputMapper>(new vecInput());

        h->setInput(t_inputMapper);
        h->reset();

    }

    if(butType == 1) {


        shared_ptr<inputMapper> t_inputMapper = shared_ptr<inputMapper>(new arcInput());

        h->setInput(t_inputMapper);
        h->reset();


    }

    if(butType == 2) {

        shared_ptr<inputMapper> t_inputMapper = shared_ptr<inputMapper>(new holdInput());
        h->setInput(t_inputMapper);
        h->reset();

    }

}

/*void testApp::modifyPath() {

    m_vessel->updateO();

    if(pathType == 0){

        shared_ptr<transTData> t_trans = shared_ptr<transTData>(new transTData());
        t_trans->dirLocal.set(0,1);
        t_trans->isBoundsFromPath = false;
        t_trans->isBoundsFromhandle = false;
        t_trans->isLocal = true;
        t_trans->isPixelBuf = true;
        t_trans->pixelBuf = 25;
        t_trans->range = 100;
        t_trans->start = 0.5;

        m_vessel->setTransform(t_trans);

    }

    if(pathType == 1){

        shared_ptr<pivotTData> t_piv = shared_ptr<pivotTData>(new pivotTData());

        t_piv->range = 180;
        t_piv->start = 0.5;
        t_piv->pivotLocal.set(ofVec2f(-0.35, 0.35));  //local position

        m_vessel->setTransform(t_piv);
    }

   // modifyhandle();

}*/


void testApp::initMoveJoint(shared_ptr <handle> h){

    //setup intial local positions for subsequent transforms

}

//--------------------------------------------------------------
void testApp::update() {

    m_worldMouse.set(mouseX - ofGetWidth()/2, -(mouseY - ofGetHeight()/2));
    vector<shared_ptr<handle> >::iterator it = m_allHandles.begin();

    while(it != m_allHandles.end()){
        (*it)->update();
        it++;
    }

    if(m_moveJoint.m_handle){

        if(m_moveJoint.m_type == "weld"){
            cout << m_moveJoint.m_handle->getPosC() << " :: ";
            cout << m_moveJoint.m_handle->getRotC() << endl;
        }

        if(m_moveJoint.m_type == "pivot"){


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
    vector<shared_ptr<handle> >::iterator it = m_allHandles.begin();

    while(it != m_allHandles.end()){
        (*it)->draw();
        it++;
    }

    //draw mouse pointer
    if(m_handle){
        (m_handle->getIsSelected() && m_handle->getIsPointInside(m_worldMouse)) ? ofSetColor(255) : ofSetColor(0);
    }else{
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
        butType = (butType + 1)%3;
        if(m_handle)modifyhandle(m_handle);
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

       m_handle.reset();
        vector<shared_ptr<handle> >::iterator it = m_allHandles.begin();

        while(it != m_allHandles.end() && !m_handle){
            if((*it)->getIsPointInside(m_worldMouse)){
                m_handle = *it;
            }
            it++;
        }

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {

     if(button == 0){
        if(m_handle)m_handle->drag(m_worldMouse);
     }

}


//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {

    if(button == 0){

        if(m_handle){
            shared_ptr<handle> ptr = m_handle->press(m_worldMouse);
            if(ptr){
                m_allHandles.push_back(ptr);
                ptr->setParent(m_handle);
                sort(m_allHandles.begin(), m_allHandles.end(), sortByLevel );
            }
        }

    }

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {

    if(button == 0){
        if(m_handle){
            m_handle->release();
        }

    }

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
