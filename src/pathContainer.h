#ifndef PATHCONTAINER_H
#define PATHCONTAINER_H

#include "pathUtils.h"
#include "utils.h"
#include "transTypes.h"

enum trDirection{

    TR_DR_NS,
    TR_DR_EW,
    TR_DR_DIAGONAL

};

class pathContainer {

public:

    pathContainer();
    virtual ~pathContainer();

    void draw();

    void start(ofVec2f b_posO, float s_val = 0);
    void update(float val);
    void updateO();

    void resizeC(int width, int height); //needs updating

    void rotateFromO(float angle, ofVec2f pivot);
    void translateFromO(ofVec2f b_pos);
    vector<ofVec2f> getIntersects(ofVec2f uv, ofVec2f p);

    ofVec2f localToWorldPoint(ofVec2f local);
    float localToWorldRot(float local);
    ofVec2f localToWorldVec(ofVec2f local);

    float getStartVal();

    //still might not need these
    ofRectangle getDims();
    ofPoint getPos();
    float getRot();

    void setTransform(shared_ptr <baseTData> b);
    ofPolyline getPolyA();

private:

    ofPolyline m_polyO, m_polyC, m_polyA; //O = stored b4 transform, C = current, A = with actual coorinates (updated with O)

    float m_rotC, m_rotO;
    ofPoint m_posC, m_posO;

    ofPoint m_bPosO;

    int m_spacing;
    float m_corners;

    shared_ptr <baseTData> m_tData;


};

#endif // PATHCONTAINER_H
