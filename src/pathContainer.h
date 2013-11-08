#ifndef PATHCONTAINER_H
#define PATHCONTAINER_H

#include "pathUtils.h"

class pathContainer
{
    public:

        pathContainer();
        virtual ~pathContainer();

        void draw();

        void updateO();

        void resizeC(int width, int height); //probably should be C
        void rotateC(float angle, ofVec2f pivot);


        ofRectangle getBoundsO();

    private:

    ofPolyline m_polyO, m_polyC;
    ofRectangle m_boundsO;

    int m_spacing;
    float m_corners;



};

#endif // PATHCONTAINER_H
