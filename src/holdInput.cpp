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
    m_bounds[0] = -1;
    m_bounds[1] = 1;

}

void holdInput::start(){

    m_isActive = true;
    m_timeStamp = ofGetSystemTime();
    m_range = m_bounds[1] - m_bounds[0];

}



void holdInput::update(ofVec2f t_mouse){

    float st = ofGetSystemTime();
    float f = (st - m_timeStamp) * m_range/(m_time * 1000);
    m_timeStamp = st;
    m_outVal += m_isAscending ? f : -f;

    if(m_isPingPong){
        if(m_outVal > m_bounds[1]){
            m_outVal = m_bounds[1];
            m_isAscending = false;
        }else if(m_outVal < m_bounds[0]){
            m_outVal = m_bounds[0];
            m_isAscending = true;
        }
    }else if(m_isLoop){
        if(m_isAscending){
            if(m_outVal > m_bounds[1])m_outVal = m_bounds[0];
        }else{
            if(m_outVal < m_bounds[0])m_outVal = m_bounds[1];
        }
    }

    m_outVal = min(m_bounds[1], max(m_outVal, m_bounds[0]));

}

void holdInput::setBounds(float f1, float f2){
    m_bounds[0] = f1;
    m_bounds[1] = f2;
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
