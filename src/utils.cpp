#include "utils.h"

namespace utils {

ofVec2f valToVec(float val, vector<ofVec2f> bnds, ofVec2f i_pos) {

    /* convert value from an input function to
    a vector based on bounds and initial position */

    ofVec2f v = bnds[1] - bnds[0];
    ofVec2f v2 = i_pos - bnds[0];
    return val * v - v2;

}

float valToRot(float val, float rng, float i_rot) {
    return(val - i_rot) * rng;
}

void genRandom(string & s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

}


}
