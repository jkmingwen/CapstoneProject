#include "ofApp.h"
#include <iostream>
#include <fstream>
//--------------------------------------------------------------
void ofApp::setup()
{
    int bufferSize = 1024; // set buffer size
    ofSoundStreamSettings settings; // create ofSoundStreamSettings object
    ofBackground(0, 0, 0);
    
    cloudDensity = 320; // set cloud density
    frictionCoef = 0.99; // set friction coefficient
    
    bufferCounter = 0;
    drawCounter = 0;
    smoothedVol = 0.0;
    scaledVol = 0.0;
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    // initialising ofSoundStreamSettings
    auto devices = soundStream.getMatchingDevices("UR22"); // <- name of input device
    if (!devices.empty()) // sets to first input device as backup
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
    myFile.open("../../../../../../Reports/Initial Thesis/data/clouds_timings.txt",
                ofFile::WriteOnly);
    lineCount = 0; // track number of lines written
}

//--------------------------------------------------------------
void ofApp::update()
{
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    // record the volume into an array
    volHistory.push_back(scaledVol);
    
    // if we are bigger than the size we want to record, we drop the oldest value
    if( volHistory.size() >= 400 )
    {
        volHistory.erase(volHistory.begin(), volHistory.begin() + 1);
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    auto start_u = std::chrono::system_clock::now();
    
//    ofDrawBitmapString(scaledVol, 20, 20);
    ofSetColor(255, 255, 255, 125);
    
    for (int particleA = 0; particleA < mass.size(); particleA++)
    {
        // initialise particle acceleration in x and y direction as 0
        ofVec2f acceleration(0.0, 0.0);
        
        for (int particleB = 0; particleB < mass.size(); particleB++)
        {
            if (particleA != particleB) // if they are different particles
            {
                // distance between particles in x and y axes
                ofVec2f coord_dist = position[particleB] - position[particleA];
                // calculate distance between particles
                float distance = position[particleB].distance(position[particleA]);
                if (distance < 1) distance = 1; // keep positive
                
                /* calculate force enacted on particle:
                 particles are generally attracted to each other
                 the moment the distance betw 2 particles is < cloudDensity,
                 they repel (i.e. they experience a negative force).
                 higher distance -> higher force */
                /* NOTE: Slight bug here: when the particles come to a stop,
                 audio input won't make them start again. Set "distance"
                 variable to be the edge of the particle (center pos + radius)
                 to fix. */
                float force = (distance - cloudDensity) * ((mass[particleB] + scaledVol) / distance);
                acceleration += (force * coord_dist);
            }
        }
        
        // at every iteration, the particle loses velocity, hence frictionCoef
        // this "friction coefficient" could be used to represent tempo
        velocity[particleA] = (velocity[particleA] * frictionCoef) + (acceleration * mass[particleA]);
    }
    
    for (int particle = 0; particle < mass.size(); particle++)
    {
        // shift particle position by its given velocity in that direction
        position[particle] += velocity[particle];
        /* if the particles are not within the dimensions of the window.
         if they're not, apply a negative velocity to the particles.
         this negative velocity is multiplied by 1.01 in order to push
         the given particle back in to the window rather than just hold
         it in position
         Potential fix:
         slowly kill particles that are on the edges for too long */
        if (position[particle].x + velocity[particle].x > ofGetWidth() ||
            position[particle].x + velocity[particle].x < 0)
        {
            position[particle].x -= 1.01 * velocity[particle].x;
        }
        else if (position[particle].y + velocity[particle].y > ofGetHeight() ||
                 position[particle].y + velocity[particle].y < 0)
        {
            position[particle].y -= 1.01 * velocity[particle].y;
        }
        
        // volume of the signal is linked to size of particle
        ofDrawEllipse(position[particle],
                      mass[particle] * 1000 + (100 * scaledVol),
                      mass[particle] * 1000 + (100 * scaledVol));
    }
    
    auto end_u = std::chrono::system_clock::now();
    auto elapsed_u = end_u - start_u;
    double total_u = elapsed_u.count() / 1000000.0; // ms duration
//    ofDrawBitmapString(total_u, 4, 60);
    
//    if (lineCount < 5000)
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
void ofApp::addNewParticle()
{
    // when adding new particle...
    mass.push_back(ofRandom(0.003, 0.03)); // set random mass
    // at position of mouse click
    position.push_back(ofVec2f(ofGetMouseX(), ofGetMouseY()));
    // no velocity (no initial speed)
    velocity.push_back(ofVec2f(0, 0));
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
    addNewParticle();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    addNewParticle();
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
void ofApp::exit()
{
    myFile.close();
}
