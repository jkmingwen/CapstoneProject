#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    windowCenter.set(ofGetWidth()/2, ofGetHeight()/2);
    circleSpacing = 3;
    // rotation = 0.0;
    int bufferSize = 256; // set buffer size
    ofSoundStreamSettings settings; // create ofSoundStreamSettings object
    ofBackground(0, 0, 0);
    
    bufferCounter = 0;
    drawCounter = 0;
    smoothedVol = 0.0;
    scaledVol = 0.0;
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    // initialising ofSoundStreamSettings
    auto devices = soundStream.getMatchingDevices("UR22"); // <- name of input device
    if (!devices.empty())
    {
        settings.setInDevice(devices[0]);
    }
    settings.setInListener(this); // current ofSoundStream as listener
    settings.sampleRate = 44100;
    settings.numOutputChannels = 0;
    settings.numInputChannels = 2; // stereo
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
    
    // create file to record timings
    myFile.open("../../../../../../Reports/Initial Thesis/data/circles_timings.txt",
                ofFile::WriteOnly);
    lineCount = 0; // track number of lines written
}

//--------------------------------------------------------------
void ofApp::update()
{
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    // record the volume into an array
    volHistory.push_back(scaledVol);
    
    // if we are bigger the the size we want to record, we drop the oldest value
    if( volHistory.size() >= 400 )
    {
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }
    // rotation += 30.0;
}

//--------------------------------------------------------------
void ofApp::draw()
{
    auto start_u = std::chrono::system_clock::now();
    
    ofSetColor(255, 255, 255);
    //ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
    //ofDrawBitmapString("Smoothed average vol (0-100): " + ofToString(smoothedVol), 4, 30);
    //string reportString = "buffers received: "+ofToString(bufferCounter)+"\n";
    //ofDrawBitmapString(reportString, 4, 60);
    
    ofPushMatrix();
    ofPushStyle(); // isolates graphics changes (i.e. colour)
    ofSetColor(200);
    ofSetLineWidth(1);
    ofNoFill();
    ofTranslate(windowCenter); // translate to window center
    ofBeginShape();
    // ofRotateZDeg(rotation);
    // use only right channel as that's where the audio interface is
    for (unsigned int i = 0; i < right.size(); i++)
    {
        ofDrawCircle(circleSpacing * (-1.0 * right.size()/2 + i),
                     -right[i]*300.0f,
                     (scaledVol * scaledVol * 150));
    }
    ofEndShape();
    ofPopStyle();
    ofPushMatrix();
    
    auto end_u = std::chrono::system_clock::now();
    auto elapsed_u = end_u - start_u;
    double total_u = elapsed_u.count() / 1000000.0; // ms duration
//    ofDrawBitmapString(total_u, 4, 60);
    
//    if (lineCount < 5000) // write 5000 timings
//    {
//        myFile << to_string(total_u) << "\n";
//        lineCount++;
//        ofDrawBitmapString(to_string(lineCount), 15, 60);
//    }
//    else
//    {
//        ofDrawBitmapString("Timings taken!", 15, 60);
//    }
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer &input)
{
    float currentVol = 0.0;
    
    int numCounted = 0;
    // calculate rms of sample volumes
    for (int i = 0; i < input.getNumFrames(); i++, numCounted+=2)
    {
        // samples are interleaved (L on even, R on odd)
        left[i] = input[i*2] * 0.5;
        right[i] = input[i*2+1] * 0.5;
        // square volumes on both channels and add them up
        currentVol += left[i] * left[i];
        currentVol += right[i] * right[i];
    }
    currentVol /= (float) numCounted; // mean
    currentVol = sqrt(currentVol); // square root
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * currentVol;
    bufferCounter++;
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
void ofApp::windowResized(int w, int h)
{
    windowCenter.set(w/2, h/2);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}

//--------------------------------------------------------------
void ofApp::exit()
{
    myFile.close();
}
