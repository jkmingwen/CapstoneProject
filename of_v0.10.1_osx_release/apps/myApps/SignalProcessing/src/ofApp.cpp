#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSoundStreamSettings settings; // create ofSoundStreamSettings object
    ofBackground(0, 0, 0);
    
    bufferCounter = 0;
    drawCounter = 0;
    smoothedVol = 0.0;
    scaledVol = 0.0;
    
    // Initialise buffer vectors
    left.resize(BUFFER_SIZE);
    right.resize(BUFFER_SIZE);
    volHistory.resize(400);
    
    // initialising ofSoundStreamSettings
    auto devices = soundStream.getMatchingDevices("UR22"); // <- name of input device
    if (!devices.empty()) // if no match, set to default
    {
        settings.setInDevice(devices[0]);
    }
    settings.setInListener(this); // current ofSoundStream as listener
    settings.sampleRate = SAMPLE_RATE;
    settings.numOutputChannels = 0;
    settings.numInputChannels = 2; // stereo
    settings.bufferSize = BUFFER_SIZE;
    soundStream.setup(settings);
    audioAnalyzer.setup(SAMPLE_RATE, BUFFER_SIZE, 2);
}

//--------------------------------------------------------------
void ofApp::update()
{
    // scaling smoothedVol between 0.0 and 1.0
    // scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    // scaledVol = ofMap(rmsVol, 0.0, 0.17, 0.0, 1.0, true);
    
    // record the volume into an array
    // volHistory.push_back(scaledVol);
    // volHistory.push_back(rmsVol);
    volHistory.push_back(rms_l);
    /* if we are bigger the the size we want to record,
     drop the oldest value. this will be the first element
     in the vector */
    if(volHistory.size() >= 400)
    {
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }
    
    smooth = ofClamp(ofGetMouseX() / ofGetWidth(), 0.0, 1.0);
    rms_l = audioAnalyzer.getValue(RMS, 0, smooth);
    rms_r = audioAnalyzer.getValue(RMS, 1, smooth);
//    rms_l = audioAnalyzer.getValue(RMS, 0);
//    rms_r = audioAnalyzer.getValue(RMS, 1);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetColor(255, 102, 178);
    ofBeginShape();
    for (int i = 0; i < volHistory.size(); i++)
    {
        if (i == 0) // start of graph
        {
            ofVertex(i, 400);
        }
        ofVertex(i, 400 - volHistory[i] * 10);
        if (i == volHistory.size() - 1) // end of graph
        {
            ofVertex(i, 400);
        }
//        ofDrawBitmapString(ofToString(rmsVol * 100), ofGetWidth()/2, ofGetHeight()/2);
        ofDrawBitmapString(ofToString(rms_l), ofGetWidth()/2, ofGetHeight()/2);
    }
    ofEndShape();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer &input)
{
//    // separate interleaved buffer into separate left/right arrays
//    for (int i = 0; i < input.getNumFrames(); i++)
//    {
//        // samples are interleaved (L on even, R on odd)
//        left[i] = input[i*2] * 0.5;
//        right[i] = input[i*2+1] * 0.5;
//    }
//    // rmsVol = RMS(left, right);
//    bufferCounter++;
    audioAnalyzer.analyze(input);
}

//--------------------------------------------------------------
//float ofApp::RMS(vector <float> left, vector <float> right)
//{
//    // Store sum of amplitudes
//    float rms = 0.0;
//    
//    /* calculate rms of sample volumes
//     BUFFER_SIZE is halved as buffer has been split
//     into L and R channels */
//    for (int i = 0; i < BUFFER_SIZE / 2; i++)
//    {
//        // square volumes on both channels add together
//        rms += left[i] * left[i];
//        rms += right[i] * right[i];
//    }
//    // divide by number of samples in buffer to get mean of squares
//    rms /= (float) BUFFER_SIZE;
//    rms = sqrt(rms); // square root
//    
////    smoothedVol *= 0.93;
////    smoothedVol += 0.07 * rms;
//    
//    return rms;
//}
