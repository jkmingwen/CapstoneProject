#pragma once

#include "ofMain.h"
#include "fftw3.h"

class ofApp : public ofBaseApp {

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
    void addNewParticle();
    void hamming(int windowLength, float *buffer);
    void STFT(vector <float> signal, int signalLength, int windowSize,
              int hopSize, vector <float> &output_result);
    
    // Function for reading audio input
    void audioIn(ofSoundBuffer &input); // ofSoundBuffer object as input
    
    // Define variables for audio
    // We create 3 separate buffers:
    vector <float> left; // left channel
    vector <float> right; // right channel
    vector <float> volHistory;

    int bufferCounter;
    int drawCounter;

    float smoothedVol;
    float scaledVol;

    ofSoundStream soundStream;

    /* Particle properties: mass, position, velocity - vectors need x and y vars
       the properties of each individual particle are stored as an element in an
       array per property */
    vector <float> mass;
    vector <float> positionX;
    vector <float> positionY;
    vector <float> velocityX;
    vector <float> velocityY;
    int cloudDensity; // this will determine how dense the particle cloud is
    float frictionCoef; // how quickly particles slow down

    // FFTW3 test variables
    int SIZE = 4; // sample size has to be powers of 2
    fftw_complex *data, *fft_result, *ifft_result;
    fftw_plan plan_forward, plan_backward;
    int i;
    vector <float> output_result;
};
