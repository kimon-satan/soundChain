#ifndef BUTTON_H
#define BUTTON_H

#include "ofxOsc.h"
#include "pathContainer.h"
#include "allInputMappers.h"

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

        ofVec2f getModMouse(); //not neeeded

        bool getIsSelected();
        bool getIsPointInside();


    private:

    ofVec2f valToVec(float val, vector<ofVec2f> bnds, ofVec2f i_pos);
    float valToRot(float val, float rng, float i_rot);

    enum transType{ // this needs to be replaced
        SNDCHN_TT_ROT_MAN,
        SNDCHN_TT_TRANS_MAN,
        SNDCHN_TT_TRANS_AUTO,
        SNDCHN_TT_SIZE_MAN,
        SNDCHN_TT_ROT_AUTO,
        SNDCHN_TT_SIZE_AUTO
    };

    float m_timeAcc;

    ofPoint m_posO, m_posC;

    float m_radius;
    bool m_isSelected, m_isMoving, m_isSoundOn, m_isPointInside;

    int m_histSize;
    vector<ofPoint> m_hist;

    transType m_transType; //this will go

    ofPoint m_pivot;
    float m_pivRange, m_pivStart;

    vector<ofVec2f> m_transBounds; //range for button translations

    ofPtr<ofxOscSender> p_sender;

    pathContainer m_pathContainer;

    shared_ptr <inputMapper> m_inputMapper;



};

#endif // BUTTON_H
