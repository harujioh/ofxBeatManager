#pragma once

#include "ofMain.h"

class ofxBeat {
   public:
    ofxBeat(float bar) : millis(ofGetElapsedTimeMillis()), bar(bar) {}

    uint64_t millis;
    float bar;
};