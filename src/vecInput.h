#ifndef VECINPUT_H
#define VECINPUT_H

#include "inputMapper.h"


class vecInput : public inputMapper
{
    public:
        vecInput();
        virtual ~vecInput();

        void reset();
        void start();
        void setStartVal(float s_val);
        void update(ofVec2f t_mouse);
        void stop();

        void setDirLocal(ofVec2f d);
        void setDirGlobal(ofVec2f d);
        ofVec2f getDirLocal();
        ofVec2f getDirGlobal();

        void setRangePixels(float f);
        void setBounds(ofVec2f t_min, ofVec2f t_max);
        ofVec2f getBounds(int i);

        void setIsLocal(bool b);
        bool getIsLocal();

    private:

    bool m_isLocal;
    float m_rangePixels;
    ofVec2f m_dirLocal, m_dirGlobal;
    ofVec2f m_bounds[2];
    float m_props[2];


};

#endif // VECINPUT_H
