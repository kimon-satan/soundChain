#ifndef BUTTON_H
#define BUTTON_H

#include "ofxOsc.h"
#include "pathContainer.h"

class button
{
    public:
        button();
        virtual ~button();


        void update();
        void draw();

        void press(ofVec2f t_mouse);
        void drag(ofVec2f t_mouse);
        void release();

        void setOSCSender( ofPtr<ofxOscSender> o);
        ofVec2f getModMouse();
        bool getIsSelected();


    private:

    float m_timeAcc;

    ofPoint m_posGlobalO, m_posGlobalC;
    float m_radius;
    bool m_isSelected, m_isMoving, m_isSoundOn;

    int m_histSize;
    vector<ofPoint> m_hist;

    ofPoint m_pivot;
    float m_pivMin, m_pivMax, m_pivCenter;

    ofPtr<ofxOscSender> p_sender;

    pathContainer m_pathContainer;

};

#endif // BUTTON_H