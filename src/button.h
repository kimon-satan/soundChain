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

        void calcIsMoving();
        void handleOSC();

        void setOSCSender( ofPtr<ofxOscSender> o);
        ofVec2f getModMouse();
        bool getIsSelected();
        bool getIsPointInside();


    private:

    enum transType{
        SNDCHN_TT_ROT_MAN,
        SNDCHN_TT_TRANS_MAN,
        SNDCHN_TT_TRANS_AUTO,
        SNDCHN_TT_SIZE_MAN,
        SNDCHN_TT_ROT_AUTO,
        SNDCHN_TT_SIZE_AUTO
    };

    float m_timeAcc;

    ofPoint m_posGlobalO, m_posGlobalC;
    float m_radius;
    bool m_isSelected, m_isMoving, m_isSoundOn, m_isPointInside;

    int m_histSize;
    vector<ofPoint> m_hist;

    transType m_transType;

    ofPoint m_pivot;
    float m_pivMin, m_pivMax, m_pivCenter;

    ofVec2f m_transDir;
    ofVec2f m_transPos, m_transNeg;

    vector<ofVec2f> m_transBounds;
    ofPtr<ofxOscSender> p_sender;

    pathContainer m_pathContainer;



};

#endif // BUTTON_H
