#ifndef handle_H
#define handle_H

#include "ofxOsc.h"
#include "vessel.h"
#include "allInputMappers.h"

class handle
{
    public:
        handle();
        virtual ~handle();

        void update();
        void draw();
        void drawSpines();

        shared_ptr<handle> press(ofVec2f t_mouse);
        void drag(ofVec2f t_mouse);
        void release();

        void reset();

        void calcIsMoving();

        void handleOSC();
        void setOSCSender( ofPtr<ofxOscSender> o);

        void setIsSelected(bool b);
        bool getIsSelected();
        bool getIsActive();
        bool getIsPointInside(ofVec2f p);

        void setInput(shared_ptr <inputMapper> p);

        ofVec2f getPosC();
        float getRotC();
        void setPos(ofVec2f p);
        void setRadius(float r);

        void setParent(shared_ptr <handle> t);
        void setHook(shared_ptr <handle> t);
        void setLevel(int i);
        int getLevel();

        shared_ptr <handle> spawnHandle();
        //spawnHandleFromParent() .. then specifiy input mapper ?

    private:

    int m_level;
    float m_timeAcc;

    ofPoint m_posO, m_posC;
    float m_rotC, m_rotO;

    float m_radius;
    bool m_isSelected, m_isActive, m_isMoving, m_isSoundOn;

    int m_histSize;

    vector<ofPoint> m_hist;

    ofPtr<ofxOscSender> p_sender;

    shared_ptr <inputMapper> m_inputMapper;

    vector <shared_ptr <handle> > m_children; //will also need to be stored in testApp
    shared_ptr <handle> m_parent, m_hook;

};

#endif // handle_H
