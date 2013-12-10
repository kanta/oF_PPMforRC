#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    
	ofBackground(34, 34, 34);
    
    oscRcv.setup(10001);
    
	sampleRate 			= 44100;    // 44100, 96000, 192000
    int bufferSize		= (TXPERIOD/1000.) * sampleRate;  //970 on 22ms & 44.1kHz
    
    startPulseSamples   = (START_PULSE/1000000.) * sampleRate;
    for (int i=0; i<nBODY; i++) {
        gui[i].setup();
        for (int j=0; j<8; j++) {
            string  lText= "body#"+ofToString(i+1)+" ch."+ofToString(j+1);
            gui[i].add(pulseLength[i][j].setup(lText, PULSE_MIN, PULSE_MIN, PULSE_MIN+PULSE_MAXLENGTH));
        }
        gui[i].add(bResetVal[i].setup("reset",false));
        
        pulseBuf[i].assign(bufferSize, 0.0);
        setInitPulseLength(i);
    }
	//soundStream.listDevices();
	
	//if you want to set the device id to be different than the default
	//soundStream.setDeviceID(1); 	//note some devices are input only and some are output only
    
	ofSetFrameRate(60);
    soundStream.setup(this, nBODY, 0, sampleRate, bufferSize, 4);
}

void testApp::setInitPulseLength(int targetBody) {
    pulseLength[targetBody][0] = PULSE_MIN;
    pulseLength[targetBody][1] = PULSE_MIN + (PULSE_MAXLENGTH/2);
    pulseLength[targetBody][2] = PULSE_MIN + (PULSE_MAXLENGTH/2);
    pulseLength[targetBody][3] = PULSE_MIN + (PULSE_MAXLENGTH/2);
    pulseLength[targetBody][4] = PULSE_MIN;
    pulseLength[targetBody][5] = PULSE_MIN;
}

//--------------------------------------------------------------
void testApp::update(){
    while (oscRcv.hasWaitingMessages()) {
        ofxOscMessage m;
        oscRcv.getNextMessage(&m);
        if (m.getAddress() == "/heliCtrl") {
            int body = m.getArgAsInt32(0);
            int ch = m.getArgAsInt32(1);
            if (ch >=0 && ch<9) {
                float nLength = m.getArgAsFloat(2);
                if (nLength < 0.) {
                    nLength = 0;
                } else if (nLength >1.) {
                    nLength = 1.;
                }
                pulseLength[body][ch] = PULSE_MIN + PULSE_MAXLENGTH * nLength;
            }
        }
    }
    for (int i=0; i<nBODY; i++) {
        if (bResetVal[i]) {
            setInitPulseLength(i);
        }
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
	ofSetColor(225);
	ofDrawBitmapString("DX7s pulse generator", 32, 32);
	ofDrawBitmapString("'e':pause Audio 's':start Audio", 31, 62);
    
    string reportString = "sampling rate: "+ofToString(sampleRate)+", buffer size: "+ofToString(pulseBuf[0].size())+"\nstart pulse samples: "+ofToString(startPulseSamples)+"\n";
    ofDrawBitmapString(reportString, 300, 32);
#define GRAPH_HEIGHT    80
    
	ofNoFill();
	
	// draw the left channel:
    
	ofPushStyle();
    for (int ch=0; ch<nBODY; ch++) {
        
		ofPushMatrix();
		ofTranslate(32, 100+(GRAPH_HEIGHT+10)*ch, 0);
        
		ofSetColor(225);
		ofDrawBitmapString("Heli #"+ofToString(ch+1), 4, 18);
		
		ofSetLineWidth(1);
		ofRect(0, 0, 900, GRAPH_HEIGHT);
        
		ofSetColor(245, 58, 135);
		ofSetLineWidth(1);
        
        ofBeginShape();
        for (unsigned int i = 0; i < pulseBuf[ch].size(); i++){
            float x =  ofMap(i, 0, pulseBuf[ch].size(), 0, 900, true);
            ofVertex(x, (GRAPH_HEIGHT/2) -pulseBuf[ch][i]*(float)(GRAPH_HEIGHT/2));
        }
        ofEndShape(false);
        
		ofPopMatrix();
    }
	ofPopStyle();
    
	ofSetColor(225);
    for (int i=0; i<nBODY; i++) {
        gui[i].draw();
    }
    
    string sampleNum;
    for (int body=0; body<nBODY; body++) {
        sampleNum = "body #"+ofToString(body+1)+"\n";
        for (int i=0; i<6; i++) {
            int pulseSamples = (pulseLength[body][i]/1000000.)*sampleRate;
            sampleNum += "ch."+ofToString(i+1)+" nSample:" + ofToString(pulseSamples)+"\n";
        }
        ofDrawBitmapString(sampleNum, 32+(body*200), 610);
        
    }
    
    
    
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	if( key == 's' ){
		soundStream.start();
	}
	
	if( key == 'e' ){
		soundStream.stop();
	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::audioOut(float * output, int bufferSize, int nChannels){
    
    for(int body=0; body<nBODY; body++) {
        int sampleCount = 0;
        for (int i = 0; i < 9; i++){
            int pulseSamples = (pulseLength[body][i]/1000000.)*sampleRate;
            for(int j=0; j<startPulseSamples; j++) {
                pulseBuf[body][sampleCount] = output[sampleCount*nChannels+body] = H_LEVEL;
                sampleCount++;
            }
            for(int j=0; j < pulseSamples; j++) {
                pulseBuf[body][sampleCount] = output[sampleCount*nChannels+body] = L_LEVEL;
                sampleCount++;
            }
        }
        while (sampleCount<bufferSize) {
            pulseBuf[body][sampleCount] = output[sampleCount*nChannels+body] = L_LEVEL;
            sampleCount++;
        }
    }
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}
