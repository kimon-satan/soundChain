#ifndef INPUTMAPPER_H
#define INPUTMAPPER_H

#include "ofMain.h"

class inputMapper
{
    public:
        inputMapper() {
            m_outVal = 0;
        }
        virtual ~inputMapper() {}

        virtual void start(){
            m_isActive = true;
        };

        virtual void update(ofVec2f t_mouse = ofVec2f(0,0)) = 0;
        virtual void stop() = 0;

        string getType(){ return m_type;}
        bool getIsUserInput(){ return m_isUserInput;}
        bool getIsActive(){return m_isActive;}
        float getVal(){ return m_outVal; }

        virtual void setStartVal(float s_val) = 0;

    protected:

        float m_outVal;
        string m_type;
        bool m_isUserInput, m_isActive;

};

#endif // INPUTMAPPER_H
