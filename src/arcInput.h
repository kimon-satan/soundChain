#ifndef ARCINPUT_H
#define ARCINPUT_H

#include "inputMapper.h"


class arcInput : public inputMapper
{
    public:
        arcInput();
        virtual ~arcInput();


        void setStartVal(float s_val);
        void update(ofVec2f t_mouse);
        void stop();

        void setPivot(ofVec2f p);
        void setPosO(ofVec2f p);
        void setRangeDegrees(float f);



    private:

        ofVec2f m_pivot, m_posO; // not sure if mouse is needed as member var
        float m_rangeDegrees;
        float m_boundsDegrees[2];
        bool m_isSmooth;

};

#endif // ARCINPUT_H
