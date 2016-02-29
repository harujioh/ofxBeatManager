#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxBeatManager.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();

    void onBeatEvent(float &bar);

   private:
    ofxOscReceiver receiver;
    ofxBeatManager beat;

    int intervalMillis = 500;
    uint64_t lastReceiverBeatEvent = 0;
};
