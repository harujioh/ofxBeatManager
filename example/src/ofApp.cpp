#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    receiver.setup(10000);

    ofAddListener(beat.onBeatEvent, this, &ofApp::onBeatEvent);
    beat.setEventIntervalBar((float)intervalMillis / 1000);
    beat.setLimitBar(10);
    beat.start();
}

//--------------------------------------------------------------
void ofApp::update() {
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage message;
        receiver.getNextMessage(message);

        if (message.getAddress() == "/bang") {
            beat.bang(message.getArgAsFloat(0));
        } else if (message.getAddress() == "/reset") {
            beat.reset();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0, 0, 0);
    if (lastReceiverBeatEvent > 0) {
        uint64_t now = ofGetElapsedTimeMillis();

        float value = 1.0f - min((float)(now - lastReceiverBeatEvent) / intervalMillis, 1.0f);

        ofBackground(255.0f * value, 0, 0);
    }
}

//--------------------------------------------------------------
void ofApp::onBeatEvent(float &bar) { lastReceiverBeatEvent = ofGetElapsedTimeMillis(); }
