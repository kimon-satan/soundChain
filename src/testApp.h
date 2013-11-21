#pragma once

#include "ofMain.h"
#include "handle.h"
#include "vessel.h"



class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

        void beginAction();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int handle);
		void mousePressed(int x, int y, int handle);
		void mouseReleased(int x, int y, int handle);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        void exit();

        void initMoveJoint(shared_ptr <handle> h);

        void modifyhandle(shared_ptr <handle> h); //might still use this

    private:

        float m_timeAcc;

        shared_ptr <handle> m_handle;
        shared_ptr <vessel> m_vessel;

        ofVec2f m_worldMouse, m_pressPos, m_dragVec;
        float m_downCount;
        bool m_isMouseDown, m_isMouseActive;
        int m_button;

        ofPtr<ofxOscSender> m_sender;

        handleJoint m_moveJoint;

        vector<shared_ptr <handle> >m_allHandles;

        int butType, pathType;

};
