#ifndef VECINPUT_H
#define VECINPUT_H

#include "inputMapper.h"


class vecInput : public inputMapper
{
    public:
        vecInput();
        virtual ~vecInput();

        void start();
        void start(float s_val);
        void update(ofVec2f t_mouse);
        void stop();

        void setDirection(ofVec2f d);
        void setPosO(ofVec2f p);
        void setRangePixels(float f);
        void setBounds(ofVec2f t_min, ofVec2f t_max);

    protected:
    private:

    float m_rangePixels;
    ofVec2f m_direction, m_posO;
    ofVec2f m_bounds[2];


};

#endif // VECINPUT_H
