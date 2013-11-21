#ifndef ARCINPUT_H
#define ARCINPUT_H

#include "inputMapper.h"


class arcInput : public inputMapper
{
    public:
        arcInput();
        virtual ~arcInput();


        void reset();
        void update(ofVec2f t_mouse);
        void stop();

        void setPivot(ofVec2f p);
        ofVec2f getPivot();
        void setBoundsDegrees(float neg, float pos);
        float getRotC();
        float getFrameRot();

    private:

        ofVec2f m_pivot; // not sure if mouse is needed as member var
        float m_rotC, m_rangeDegrees;
        float m_boundsDegrees[2], m_props[2];
        bool m_isSmooth;
        float m_frameRot;

};

#endif // ARCINPUT_H
