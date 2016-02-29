#pragma once

#include "ofMain.h"

class ofxBeat {
   public:
    ofxBeat(float bar) : millis(ofGetElapsedTimeMillis()), bar(bar) {}
    ofxBeat(uint64_t millis, float bar, float speed) : millis(millis), bar(bar), speed(speed) {}

    uint64_t millis;
    float bar;
    float speed;
};