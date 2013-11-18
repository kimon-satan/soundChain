#ifndef BUTTON_H
#define BUTTON_H

#include "ofxOsc.h"
#include "pathContainer.h"
#include "allInputMappers.h"

class button
{
    public:
        button(shared_ptr<pathContainer> p);
        virtual ~button();

        void update();
        void draw();

        void press(ofVec2f t_mouse);
        void drag(ofVec2f t_mouse);
        void release();

        void reset();

        void calcIsMoving();

        void handleOSC();
        void setOSCSender( ofPtr<ofxOscSender> o);

        ofVec2f getModMouse(); //not neeeded

        bool getIsSelected();
        bool getIsPointInside();

        void setPathContainer(shared_ptr<pathContainer> p);

        void setInput(shared_ptr <inputMapper> p);
        void setTransform(shared_ptr <baseTData> b);

        void reconcileStartVals();

        ofVec2f getPos();


    private:

    float m_timeAcc;

    ofPoint m_posO, m_posC;

    float m_radius;
    bool m_isSelected, m_isMoving, m_isSoundOn, m_isPointInside;

    int m_histSize;
    vector<ofPoint> m_hist;

    ofPtr<ofxOscSender> p_sender;

    shared_ptr <pathContainer> m_pathContainer;
    shared_ptr <baseTData> m_tData;
    shared_ptr <inputMapper> m_inputMapper;

};

#endif // BUTTON_H
