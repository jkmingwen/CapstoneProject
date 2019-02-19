#pragma once

#include "ofMain.h"

class Line
{
public:
    ofPoint a;
    ofPoint b;
};

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
    float RMS(vector <float> left, vector <float> right);
    
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
    
    vector <ofVec2f> vecField;
    vector <ofVec2f> vecFieldRot;
    vector <ofVec2f> particle;
    int zoff;
    float s = 0.001;
    int fieldDensity = 15;
    
    vector <ofVec2f> position;
    vector <ofVec2f> velocity;
    vector <ofVec2f> force;
    vector <ofVec2f> part_flow;
    
    ofPolyline line;
};
