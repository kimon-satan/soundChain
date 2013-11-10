#ifndef PATHCONTAINER_H
#define PATHCONTAINER_H

#include "pathUtils.h"

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

    void updateO();

    void resizeC(int width, int height); //needs updating

    void rotateMan(float angle, ofVec2f pivot);
    void translateMan(ofVec2f b_pos);
    vector<ofVec2f> getIntersects(ofVec2f uv, ofVec2f p);
    ofVec2f getLocal(ofVec2f world);

    //still might not need these
    ofRectangle getBoundsO();
    ofPoint getPos();
    float getRot();

private:

    ofPolyline m_polyO, m_polyC, m_polyA; //O = stored b4 transform, C = current, A = with actual coorinates (updated with O)

    float m_rotC, m_rotO;
    ofPoint m_posC, m_posO;

    int m_spacing;
    float m_corners;



};

#endif // PATHCONTAINER_H
