#include "arcInput.h"

arcInput::arcInput()
{
    //ctor
    m_type = "arcInput";
    m_isUserInput = true;
    m_isSmooth = true;

}


void arcInput::setStartVal(float s_val){

    m_outVal = s_val;

    //construct the arc
    ofVec2f v = m_posO - m_pivot;

    m_boundsDegrees[0] = - s_val * m_rangeDegrees;
    m_boundsDegrees[1] = (1 - s_val) * m_rangeDegrees;

}

void arcInput::update(ofVec2f t_mouse){

    float p_val = m_outVal;
    ofVec2f v = t_mouse - m_pivot;
    ofVec2f vO = m_posO - m_pivot;
    float ang = vO.angle(v);

    if(ang < 0){

        ang = max(m_boundsDegrees[0], ang);
        m_outVal = -(m_boundsDegrees[0] - ang)/m_rangeDegrees;

    }else{

        ang = min(m_boundsDegrees[1], ang);
        m_outVal = (-m_boundsDegrees[0] + ang)/m_rangeDegrees;

    }

    if(m_isSmooth){
        m_outVal = max(p_val - 0.01f, min( p_val + 0.01f, m_outVal)); //no movements bigger than 0.01 per frame
    }

}

void arcInput::stop(){

    //is anything needed here ?
    //perhaps resets if allowing more than 180 degs on either side of sp

}

//extra getters and setters

void arcInput::setPosO(ofVec2f p){m_posO.set(p);}
void arcInput::setPivot(ofVec2f p){m_pivot.set(p);}
void arcInput::setRangeDegrees(float f){m_rangeDegrees = f;}


arcInput::~arcInput()
{
    //dtor
}
