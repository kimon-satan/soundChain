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

    void translateAuto(ofVec2f t_vec, vector<ofVec2f> bounds);

    ofVec2f localToWorldPoint(ofVec2f local);
    float localToWorldRot(float local);
    ofVec2f localToWorldVec(ofVec2f local);

    void setBounds(ofVec2f pos, ofVec2f v, float prop);
    void setBounds(ofVec2f pos, ofVec2f v, int bufPixels);

    void setBoundsFromPath(vector<ofVec2f> & bnds, ofVec2f pos, ofVec2f v, float prop);
    void setBoundsFromPath(vector<ofVec2f> & bnds, ofVec2f pos, ofVec2f v, int bufPixels);

    vector<ofVec2f> getBounds();

    //still might not need these
    ofRectangle getDims();
    ofPoint getPos();
    float getRot();



private:

    ofPolyline m_polyO, m_polyC, m_polyA; //O = stored b4 transform, C = current, A = with actual coorinates (updated with O)

    float m_rotC, m_rotO;
    ofPoint m_posC, m_posO;

    vector<ofVec2f> m_bounds;

    int m_spacing;
    float m_corners;



};

#endif // PATHCONTAINER_H
