#pragma once

#include "ofMain.h"
#include "ofxBeat.h"

// ビートを保存しておく数
static const int KEEP_BEAT_LENGTH = 100;

// 小説を丸める時の有効桁数
static const int EFFECTIVE_DIGIT_FOR_ROUND_BAR = 3;

/**
 * テンポの管理をおこなうマネージャ
 */
class ofxBeatManager : public ofThread {
   public:
    ofxBeatManager() { initVar(); }
    ~ofxBeatManager();

    // setter
    void setLimitBar(float limitBar) { this->limitBar = limitBar; }
    void setEventIntervalBar(float eventIntervalBar) { this->eventIntervalBar = eventIntervalBar; }

    // getter
    float getUpdateBar() { return updateBar; }

    // thread
    void start();
    void reset();
    void threadedFunction();

    // beat
    float bang(float bar);

    // event handler
    ofEvent<ofxBeat> onBeatEvent;

   private:
    void startThread();
    void threadedUpdate();

    // event interval
    float limitBar = -1;
    float eventIntervalBar = -1;

    // var
    vector<ofxBeat *> beats;
    ofxBeat *lastReceivedBeat;
    float newestIntervalBar;
    float speed;
    float updateBar, lastUpdateBar;
    void initVar();
};