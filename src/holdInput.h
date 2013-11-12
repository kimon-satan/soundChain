#ifndef HOLDINPUT_H
#define HOLDINPUT_H

#include "inputMapper.h"


class holdInput : public inputMapper
{
    public:
        holdInput();
        virtual ~holdInput();

        void start(); //without reset
        void start(float s_val);
        void update(ofVec2f t_mouse = ofVec2f(0,0));
        void stop();

        void setIsAscending(bool b);
        void setIsPingPong(bool b);
        void setIsLoop(bool b);
        void setTime(float s);
        void setAttSecs(float f);
        void setDecSecs(float f);

    private:

        bool m_isAscending, m_isPingPong, m_isLoop; //potentially other variations here aswell
        float m_time, m_attSecs, m_decSecs;
};

#endif // HOLDINPUT_H
