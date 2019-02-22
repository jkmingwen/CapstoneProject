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
    
    // initialise flow field
    for (int x = 1; x <= ofGetWidth(); x+= fieldDensity)
    {
        for (int y = 1; y <= ofGetHeight(); y += fieldDensity)
        {
            vecField.push_back(ofVec2f(x, y)); // fill vector field with empty points
            vecFieldRot.push_back(ofVec2f(x, y-fieldDensity)); // rotation of each point
        }
    }
    // initialise particles
//    for (int i = 0; i < 50; i++)
//    {
//        position.push_back(ofVec2f(ofRandom(ofGetWidth(), ofGetHeight())));
//        velocity.push_back(ofVec2f(0, 0));
//        force.push_back(ofVec2f(0, 0));
//        part_flow.push_back(ofVec2f(0, -1));
//    }
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
    for (int i = 0; i <= vecField.size() -1; i++)
    {
        float flow = ofSignedNoise(vecField[i].x * s, vecField[i].y * s, zoff * s);
        vecFieldRot[i].rotate(flow + (rmsVol * 100), vecField[i]);
    }
    for (int i = 0; i <= vecFieldRot.size() - 1; i++)
    {
        float flow = ofSignedNoise(vecField[i].x * s, vecField[i].y * s, zoff * s);
        ofSetColor(255, 255, 255, ofMap(flow, -1.0, 1.0, 0, 255));
        ofDrawLine(vecField[i], vecFieldRot[i]);
    }
    
    //    for (int i = 0; i < 50 - 1; i++)
    //    {
    //        float flow = ofSignedNoise(position[i].x * s, position[i].y * s, zoff * s);
    //        part_flow[i].set(0, -1);
    //        part_flow[i].rotate(ofMap(flow, -1.0, 1.0, 0, 90), position[i]);
    //
    //        line.addVertex(position[i].x, position[i].y);
    //
    //        for (int j = 0; j < 50; j++)
    //        {
    //            if (j != i)
    //            {
    //                if (position[i].distance(position[j]) < 500)
    //                {
    //                    line.addVertex(position[j].x, position[j].y);
    //                }
    //            }
    //        }
    //
    //        force[i] += part_flow[i];
    //        force[i] *= 0.01;
    //        velocity[i] += force[i];
    //        velocity[i].normalize();
    //        position[i] += velocity[i];
    //        force[i] *= 0;
    //
    //    }
    // line.draw();
    zoff++;
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
void ofApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
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
    // update flow field after resize
    for (int x = 1; x <= ofGetWidth(); x+= fieldDensity)
    {
        for (int y = 1; y <= ofGetHeight(); y += fieldDensity)
        {
            vecField.push_back(ofVec2f(x, y)); // fill vector field with empty points
            vecFieldRot.push_back(ofVec2f(x, y-fieldDensity)); // rotation of each point
        }
    }
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
    // separate interleaved buffer into separate left/right arrays
    for (int i = 0; i < input.getNumFrames(); i++)
    {
        // samples are interleaved (L on even, R on odd)
        left[i] = input[i*2] * 0.5;
        right[i] = input[i*2+1] * 0.5;
    }
    rmsVol = RMS(left, right);
    bufferCounter++;
}

//--------------------------------------------------------------
float ofApp::RMS(vector <float> left, vector <float> right)
{
    // Store sum of amplitudes
    float rms = 0.0;
    
    /* calculate rms of sample volumes
     BUFFER_SIZE is halved as buffer has been split
     into L and R channels */
    for (int i = 0; i < BUFFER_SIZE / 2; i++)
    {
        // square volumes on both channels add together
        rms += left[i] * left[i];
        rms += right[i] * right[i];
    }
    // divide by number of samples in buffer to get mean of squares
    rms /= (float) BUFFER_SIZE;
    rms = sqrt(rms); // square root
    
    //    smoothedVol *= 0.93;
    //    smoothedVol += 0.07 * rms;
    
    return rms;
}
