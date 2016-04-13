#include "ofxBeatManager.h"

// destructor
ofxBeatManager::~ofxBeatManager() {
    stopThread();
    while (isThreadRunning())
        ;

    for (int i = 0; i < beats.size(); i++) {
        delete beats[i];
    }
}

// init var
void ofxBeatManager::initVar() {
    for (int i = 0; i < beats.size(); i++) {
        delete beats[i];
    }
    beats.clear();

    lastReceivedBeat = NULL;
    speed = 0;
    newestIntervalBar = -1;
    lastUpdateBar = -1;
}

// thread start
void ofxBeatManager::start() {
    initVar();

    this->ofThread::startThread();
}

// thread reset
void ofxBeatManager::reset() {
    if (lock()) {
        initVar();

        unlock();
    }
}

// thread main
void ofxBeatManager::threadedFunction() {
    while (isThreadRunning()) {
        if (lock()) {
            threadedUpdate();
            unlock();
            sleep(1);
        }
    }
}

// threadUpdate
void ofxBeatManager::threadedUpdate() {
    if (lastReceivedBeat == NULL || speed <= 0) {
        return;
    }

    uint64_t now = ofGetElapsedTimeMillis();
    updateBar = lastReceivedBeat->bar + (float)(now - lastReceivedBeat->millis) / speed;

    if (lastUpdateBar >= 0 && eventIntervalBar > 0) {
        float floorBar = floor(updateBar / eventIntervalBar) * eventIntervalBar;
        float lastUpdateFloorBar = floor(lastUpdateBar / eventIntervalBar) * eventIntervalBar;

        // hook event
        if (lastUpdateFloorBar != floorBar) {
            if (limitBar < 0 || floorBar < limitBar) {
                ofxBeat *beat = new ofxBeat(now, floorBar, speed);
                ofNotifyEvent(onBeatEvent, *beat);
                delete beat;
            }
        }
    }
    lastUpdateBar = updateBar;
}

// bang
float ofxBeatManager::bang(float bar) {
    if (lastReceivedBeat != NULL && lastReceivedBeat->bar == bar) {
        return 1000 / (speed * newestIntervalBar / 60);
    }

    while (beats.size() >= KEEP_BEAT_LENGTH) {
        delete beats[0];
        beats.erase(beats.begin());
    }

    lastReceivedBeat = new ofxBeat(bar);
    beats.push_back(lastReceivedBeat);

    if (beats.size() >= 2) {
        for (int i = beats.size() - 1; i > 0; i--) {
            if (beats[i]->bar > beats[i - 1]->bar) {
                newestIntervalBar = beats[i]->bar - beats[i - 1]->bar;
                break;
            }
        }
        if (newestIntervalBar < 0) {
            return 1000 / (speed * newestIntervalBar / 60);
        }

        float millisPerBarSum = 0;
        int millisPerBarTotal = 0;
        for (int i = 0; i < beats.size() - 1; i++) {
            float intervalBar = beats[i + 1]->bar - beats[i]->bar;
            float intervalMillis = beats[i + 1]->millis - beats[i]->millis;

            if (intervalBar > 0 && intervalMillis > 0) {
                millisPerBarSum += intervalMillis / intervalBar;
                millisPerBarTotal++;
            }
        }
        speed = millisPerBarSum / millisPerBarTotal;
    }

    return 1000 / (speed * newestIntervalBar / 60);
}