#include "arcInput.h"

arcInput::arcInput()
{
    //ctor
    m_type = "arcInput";
    m_isUserInput = true;
    m_isSmooth = true;

}


void arcInput::reset(){

    m_outVal = 0;

    //construct the arc
    ofVec2f v = m_posO - m_pivot;
    m_rangeDegrees = abs(m_boundsDegrees[1] - m_boundsDegrees[0]);

    for(int i = 0; i < 2; i++){
        m_props[i] = abs(m_boundsDegrees[i]/m_boundsDegrees[(i+1)%2]);
        m_props[i] = min(m_props[i],1.0f);
    }

    //set m_rotC
    m_rotC = 0;
    m_posC = m_posO;

}

void arcInput::update(ofVec2f t_mouse){

    ofVec2f v = t_mouse - m_pivot;
    ofVec2f vO = m_posO - m_pivot;
    float ang = vO.angle(v);

    ang = max(m_rotC - 4, min( m_rotC + 4, ang));

    if(ang < 0){

        ang = max(m_boundsDegrees[0], ang);
        m_outVal =  ofMap(ang, 0, m_boundsDegrees[0], 0, -m_props[0]);

    }else{

        ang = min(m_boundsDegrees[1], ang);
        m_outVal =  ofMap(ang, 0, m_boundsDegrees[1], 0, m_props[1]);

    }

    m_posC = m_posO.getRotated(ang, m_pivot);
    m_rotC = ang;

}

void arcInput::stop(){

    //is anything needed here ?
    //perhaps resets if allowing more than 180 degs on either side of sp

}

//extra getters and setters

void arcInput::setPivot(ofVec2f p){m_pivot.set(p);}
ofVec2f arcInput::getPivot(){return m_pivot;}
void arcInput::setBoundsDegrees(float neg, float pos){
    m_boundsDegrees[0] = neg;
    m_boundsDegrees[1] = pos;
}
float arcInput::getRotC(){return m_rotC;}

arcInput::~arcInput()
{
    //dtor
}
