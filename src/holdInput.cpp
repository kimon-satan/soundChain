#include "holdInput.h"

holdInput::holdInput()
{
    //ctor
    m_type = "holdInput";
    m_isUserInput = false;
    m_isAscending = true;
    m_isPingPong = false;
    m_isLoop = false;
    m_outVal = 0;
    m_isActive = false;

}

void holdInput::start(){

    m_isActive = true;

}

void holdInput::start(float s_val){

    m_isActive = true;
    m_outVal = s_val;

}

void holdInput::update(ofVec2f t_mouse){

    float f = ofGetLastFrameTime()/m_time;
    m_outVal += m_isAscending ? f : -f;

    if(m_isPingPong){
        if(m_outVal > 1){
            m_outVal = 1;
            m_isAscending = false;
        }else if(m_outVal < 0){
            m_outVal = 0;
            m_isAscending = true;
        }
    }else if(m_isLoop){
        if(m_isAscending){
            if(m_outVal > 1)m_outVal = 0;
        }else{
            if(m_outVal < 0)m_outVal = 1;
        }
    }

    m_outVal = min(1.0f, max(m_outVal, 0.0f));

}

void holdInput::stop(){
    m_isActive = false;
}

void holdInput::setIsAscending(bool b){
    m_isAscending = b;
}

void holdInput::setIsPingPong(bool b){
    m_isPingPong = b;
}
void holdInput::setIsLoop(bool b){
    m_isLoop = b;
}
void holdInput::setTime(float s){
    m_time = s;
}
void holdInput::setAttSecs(float f){
    m_attSecs = f;
}

void holdInput::setDecSecs(float f){
    m_decSecs = f;
}

holdInput::~holdInput()
{
    //dtor
}
