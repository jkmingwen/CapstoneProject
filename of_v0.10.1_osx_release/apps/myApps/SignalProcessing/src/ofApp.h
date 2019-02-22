#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzer.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void audioIn(ofSoundBuffer &input);
    // float RMS(vector <float> left, vector <float> right);
    
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    int BUFFER_SIZE = 256;
    int SAMPLE_RATE = 44100;
    
    int bufferCounter;
    int drawCounter;
    
    float smoothedVol;
    float scaledVol;
    float rmsVol;
    
    ofSoundStream soundStream;
    ofxAudioAnalyzer audioAnalyzer;
    
    float smooth;
    float rms_l, rms_r;
};
