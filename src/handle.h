#ifndef handle_H
#define handle_H

#include "ofxOsc.h"
#include "allInputMappers.h"
#include "utils.h"

class handle;

struct handleJoint{

    shared_ptr <handle> m_handle;
    string m_type;
    float m_rot;
    ofVec2f m_trans;

};

enum handleAction{

    HA_VEC_SPAWN,
    HA_OTHER

};


class handle
{
    public:
        handle();
        virtual ~handle();

        void update();
        void draw();
        void drawSpines();

        shared_ptr<handle> press(ofVec2f t_vec, int ha);
        void drag(ofVec2f t_mouse);
        void release();
        void reset();

        void pivot(float f, ofVec2f p, ofVec2f t, string jType, string actor);

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

        string getIndex();
        string getInputType();

        shared_ptr <handle> spawnHandle();
        //spawnHandleFromParent() .. then specifiy input mapper ?

        handleJoint getChildJoint(string index);
        void setChildJoint(string index, string jt);

        string getJointType();
        void setJointType( string jt);


    private:

    int m_level;
    string m_index;

    ofPoint m_posC;
    float m_rotC;

    float m_radius;
    bool m_isSelected, m_isActive, m_isMoving, m_isSoundOn;

    int m_histSize;

    vector<ofPoint> m_hist;

    ofPtr<ofxOscSender> p_sender;

    shared_ptr <inputMapper> m_inputMapper;

    vector <handleJoint> m_children; //will also need to be stored in testApp
    shared_ptr <handle> m_parent, m_hook;

};

#endif // handle_H
