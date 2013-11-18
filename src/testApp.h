#pragma once

#include "ofMain.h"
#include "button.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        void exit();

        void modifyButton();
        void modifyPath();

    private:

        shared_ptr <button> m_button;
        shared_ptr <pathContainer> m_pathContainer;
        ofVec2f m_worldMouse, m_drawMouse;
        ofPtr<ofxOscSender> m_sender;

        int butType, pathType;

};
