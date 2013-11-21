#ifndef vessel_H
#define vessel_H

#include "pathUtils.h"
#include "utils.h"
#include "transTypes.h"

enum trDirection{

    TR_DR_NS,
    TR_DR_EW,
    TR_DR_DIAGONAL

};

class vessel {

public:

    vessel();
    virtual ~vessel();

    void draw();

    void update(float val);
    void updateO();

    ofVec2f localToWorldPoint(ofVec2f local);
    float localToWorldRot(float local);
    ofVec2f localToWorldVec(ofVec2f local);

    ofRectangle getDims();
    ofPoint getPos();
    void setPos(ofPoint p);
    float getRot();
    void setRot(float f);

    ofPolyline getPolyA();

private:

    ofPolyline m_polyO, m_polyC, m_polyA; // C = current, A = with actual coorinates (updated with O)

    float m_rotC;
    ofPoint m_posC;

    int m_spacing;
    float m_corners;


};

#endif // vessel_H
