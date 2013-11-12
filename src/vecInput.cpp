#include "vecInput.h"

vecInput::vecInput() {
    //ctor
    m_type = "vecInput";
    m_isUserInput = true;

}


void vecInput::start(){

    //bounds have already been set ... do nothing

}

void vecInput::start(float s_val) {

    m_bounds[0] = m_posO - (m_direction * m_rangePixels * s_val);
    m_bounds[1] = m_posO + (m_direction * m_rangePixels * (1-s_val));

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

    m_outVal = ofVec2f(p - m_bounds[0]).length()/m_rangePixels;

}

void vecInput::stop() {

    //nothing here

}


void vecInput::setBounds(ofVec2f t_min, ofVec2f t_max){

    //setting bounds sets up everything
    m_bounds[0].set(t_min);
    m_bounds[1].set(t_max);
    ofVec2f v(t_max - t_min);
    m_direction.set(v.getNormalized());
    m_rangePixels = v.length();

}

void vecInput::setDirection(ofVec2f d) {
    m_direction.set(d);
}
void vecInput::setPosO(ofVec2f p) {
    m_posO.set(p);
}
void vecInput::setRangePixels(float f) {
    m_rangePixels = f;
}

vecInput::~vecInput() {
    //dtor
}
