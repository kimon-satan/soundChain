#ifndef TRANSTYPES_H
#define TRANSTYPES_H

#include "ofMain.h"

class baseTData {
public:
    baseTData() {};
    virtual ~baseTData() {};
    string type;
    float range, start;

};


class transTData : public baseTData {
public:
    transTData(){ type = "translate"; };
    vector<ofVec2f> bounds;
    ofVec2f dirLocal, dirGlobal;
    bool isLocal, isBoundsFromPath, isBoundsFromhandle, isPixelBuf;
    int pixelBuf;
    float prop;

};


class pivotTData : public baseTData {
public:

    pivotTData(){ type = "pivot"; };
    ofVec2f pivotLocal, pivotGlobal;
    bool isLocal;
};

//more to follow here ...

#endif // TRANSTYPES_H
