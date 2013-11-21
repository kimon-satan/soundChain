#include "vecInput.h"

vecInput::vecInput() {
    //ctor
    m_type = "vecInput";
    m_isUserInput = true;
    m_isLocal = true;
    m_dirLocal.set(1,0);

}

void vecInput::reset(){

    float t_lengths[2];

    for(int i = 0; i < 2; i ++){
        t_lengths[i] = ofVec2f(m_bounds[i] - m_posO).length();
    }

    for(int i = 0; i < 2; i ++){
        m_props[i] = t_lengths[i]/t_lengths[(i+1)%2];
        m_props[i] = min(m_props[i],1.0f);
    }



}

void vecInput::start(){

    //bounds have already been set ... do nothing

}

void vecInput::setStartVal(float s_val) { // not quite right sort out in a bit

    m_bounds[0] = m_posO - (m_dirGlobal * m_rangePixels * s_val);
    m_bounds[1] = m_posO + (m_dirGlobal * m_rangePixels * (1-s_val));

}

void vecInput::update(ofVec2f t_mouse) {

    ofVec2f v = m_bounds[1] - m_bounds[0];

    ofVec2f p;

    //project mouse onto the vector
    float k = (v.y * (t_mouse.x - m_bounds[0].x) - v.x * (t_mouse.y - m_bounds[0].y)) / (pow(v.y, 2) + pow(v.x, 2));

    p.x = t_mouse.x - k * v.y;
    p.y = t_mouse.y + k * v.x;

    //constrain the proportions
    p.x = max(p.x, min(m_bounds[0].x, m_bounds[1].x));
    p.x = min(p.x, max(m_bounds[0].x, m_bounds[1].x));
    p.y = max(p.y, min(m_bounds[0].y, m_bounds[1].y));
    p.y = min(p.y, max(m_bounds[0].y, m_bounds[1].y));

    m_posC = p;

    ofVec2f v2 = ofVec2f(p - m_posO).getNormalized();


    float ang =  v2.angle(m_dirGlobal.getPerpendicular());
    int index = (ang > 0) ? 0 : 1;
    int sign = (index == 0) ? -1 : 1;

    m_outVal = ofVec2f(p - m_posO).length()/ ofVec2f(m_bounds[index] - m_posO).length() * m_props[index] * sign;

}

void vecInput::stop() {

    //nothing here

}


void vecInput::setBounds(ofVec2f t_min, ofVec2f t_max){

    //setting bounds sets up everything
    m_bounds[0].set(t_min);
    m_bounds[1].set(t_max);
    ofVec2f v(t_max - t_min);
    m_dirGlobal.set(v.getNormalized());
    m_rangePixels = v.length();

}

ofVec2f vecInput::getBounds(int i){return m_bounds[i];}

void vecInput::setDirGlobal(ofVec2f d) {
    m_dirGlobal.set(d);
}

void vecInput::setDirLocal(ofVec2f d) {
    m_dirLocal.set(d);
}

ofVec2f vecInput::getDirLocal(){
    return m_dirLocal;
}

ofVec2f vecInput::getDirGlobal(){
    return m_dirGlobal;
}


void vecInput::setRangePixels(float f) {
    m_rangePixels = f;
}


void vecInput::setIsLocal(bool b){ m_isLocal = b; }
bool vecInput::getIsLocal(){return m_isLocal; }

vecInput::~vecInput() {
    //dtor
}
