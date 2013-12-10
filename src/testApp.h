#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"

#define nBODY 2 // number of Helicopters

#define TXPERIOD    22  //ms
#define START_PULSE 430 //us
#define PULSE_MIN   700 //us    650
#define PULSE_MAXLENGTH 860 //us
#define H_LEVEL -1.0 //1.0 for DX7
#define L_LEVEL 0.2  //-0.2 for DX7


class testApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void setInitPulseLength(int targetBody);
    
    void audioOut(float * input, int bufferSize, int nChannels);
    
    
    ofSoundStream soundStream;
    int		sampleRate;
    vector<float> pulseBuf[nBODY];
    int startPulseSamples;
    ofxPanel    gui[nBODY];
    ofxLabel    bodyNum;
    ofxIntSlider p1Length;
    ofxIntSlider pulseLength[nBODY][9];
    ofxButton   bResetVal[nBODY];
    ofxOscReceiver  oscRcv;
    
    
    
};
