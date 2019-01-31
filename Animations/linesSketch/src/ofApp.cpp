#include "ofApp.h"
#include "fftw3.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    int bufferSize = 256; // set buffer size
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

    // FFTW3 test setup
    data = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * SIZE);
    fft_result = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * SIZE);
    ifft_result = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * SIZE);

    plan_forward = fftw_plan_dft_1d(SIZE, data, fft_result,
				    FFTW_FORWARD, FFTW_ESTIMATE);
    plan_backward = fftw_plan_dft_1d(SIZE, data, ifft_result,
				     FFTW_BACKWARD, FFTW_ESTIMATE);

    // FFTW3 Test
    for(i = 0 ; i < SIZE ; i++)
    {
	data[i][0] = 1.0; // stick your audio samples in here
	data[i][1] = 0.0; // use this if your data is complex valued
    }
 

    for(i = 0; i < SIZE; i++)
    {
	fprintf(stdout, "data[%d] = { %2.2f, %2.2f }\n",
		i, data[i][0], data[i][1]);
    }
 
    fftw_execute(plan_forward);
 

    for(i = 0; i < SIZE; i++)
    {
	fprintf(stdout, "fft_result[%d] = { %2.2f, %2.2f }\n",
		i, fft_result[i][0], fft_result[i][1]);
    }
 
    fftw_execute(plan_backward);
 

    for(i = 0; i < SIZE; i++)
    {
	fprintf(stdout, "ifft_result[%d] = { %2.2f, %2.2f }\n",
		i, ifft_result[i][0] / SIZE, ifft_result[i][1] / SIZE);
    }
 

    fftw_destroy_plan(plan_forward);
    fftw_destroy_plan(plan_backward);
 
    fftw_free(data);
    fftw_free(fft_result);
    fftw_free(ifft_result);
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
        volHistory.erase(volHistory.begin(), volHistory.begin() + 1);
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetColor(255, 255, 255);

    for (int particleA = 0; particleA < mass.size(); particleA++)
    {
	// initialise particle acceleration in x and y direction as 0
        int accelerationX = 0;
        int accelerationY = 0;
        
        for (int particleB = 0; particleB < mass.size(); particleB++)
	{
            if (particleA != particleB) // if they are different particles
	    {
		// distance between particles in x and y axes
                int distanceX = positionX[particleB] - positionX[particleA];
                int distanceY = positionY[particleB] - positionY[particleA];
		// using pythagoras' theorem, calculate distance betw particles
                int distance = sqrt(distanceX * distanceX + distanceY * distanceY);
                if (distance < 1) distance = 1; // keep positive

		/* calculate force enacted on particle:
		   particles are generally attracted to each other
		   the moment the distance betw 2 particles is < cloudDensity,
		   they repel (i.e. they experience a negative force). 
		   higher distance -> higher force */
                float force = (distance - cloudDensity) *
		    ((mass[particleB] + scaledVol) / distance);
                accelerationX += force * distanceX;
                accelerationY += force * distanceY;
            }
        }

	// at every iteration, the particle loses velocity, hence frictionCoef
	// this "friction coefficient" could be used to represent tempo
	velocityX[particleA] = (velocityX[particleA] * frictionCoef +
				accelerationX * mass[particleA]);
	velocityY[particleA] = (velocityY[particleA] * frictionCoef +
				accelerationY * mass[particleA]);
    }
    
    for (int particle = 0; particle < mass.size(); particle++)
    {
	// shift particle position by its given velocity in that direction
	positionX[particle] += velocityX[particle];
	positionY[particle] += velocityY[particle];
		
	/* if the particles are not within the dimensions of the window.
	   if they're not, apply a negative velocity to the particles. 
	   this negative velocity is multiplied by 2 in order to push
	   the given particle back in to the window rantehr than just hold
	   it in position 
	   Potential fix: 
	   slowly kill particles that are on the edges for too long */
	if (positionX[particle] + velocityX[particle] > ofGetWidth() ||
	    positionX[particle] + velocityX[particle] < 0)
	{
	    positionX[particle] -= 2 * velocityX[particle];
	}
	else if (positionY[particle] + velocityY[particle] > ofGetWidth() ||
		 positionY[particle] + velocityY[particle] < 0)
	{
	    positionY[particle] -= 2 * velocityY[particle];
	}
        
        // right now the volume of the signal is linked only to size of particle
        ofDrawEllipse(positionX[particle],
		      positionY[particle],
		      mass[particle] * 1000 + (100 * scaledVol),
		      mass[particle] * 1000 + (100 * scaledVol));
    }
    
    
}

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
void ofApp::keyPressed(int key)
{
    // Starts and stops sound stream
    // Useful for demonstrating live aspect
    if( key == 's' )
    {
        soundStream.start();
    }
    
    if( key == 'e' )
    {
        soundStream.stop();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

void ofApp::addNewParticle()
{
    // when adding new particle...
    mass.push_back(ofRandom(0.003, 0.03)); // set random mass
    // at position of mouse click
    positionX.push_back(ofGetMouseX());
    positionY.push_back(ofGetMouseY());
    // no velocity (no initial speed)
    velocityX.push_back(0);
    velocityY.push_back(0);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    addNewParticle();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
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
// Creates a hamming window of windowLength samples in buffer
void ofApp::hamming(int windowLength, float *buffer)
{
    for (int i = 0; i < windowLength; i++)
    {
	buffer[i] = 0.54 - (0.46 * cos(2 * PI * (i / ((windowLength - 1) * 1.0))));
    }
}

//--------------------------------------------------------------
void ofApp::STFT(vector <float> *signal, int signalLength,
		 int windowSize, int hopSize, vector <float> output_result)
{
    fftw_complex    *data, *fft_result, *ifft_result;
    fftw_plan       plan_forward, plan_backward;
    int             i;

    /* data: stores data we want to perform FFT on
       fft_result: stores results of FFT on real values
       ifft_result: stores results of FFT on imaginary values */
    data = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * windowSize);
    fft_result = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * windowSize);
    ifft_result = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * windowSize);
 
    plan_forward = fftw_plan_dft_1d(windowSize, data, fft_result, FFTW_FORWARD, FFTW_ESTIMATE);
 
    // Create a hamming window of appropriate length
    float window[windowSize];
    hamming(windowSize, window);
 
    int chunkPosition = 0;
    int readIndex;
    // Should we stop reading in chunks? 
    int bStop = 0;
    
    // Process each chunk of the signal
    while(chunkPosition < signalLength && !bStop)
    {
	// Copy the chunk into our buffer
	for(i = 0; i < windowSize; i++)
	{
	    readIndex = chunkPosition + i;
	    if(readIndex < signalLength)
	    {
		// Window signal by multiplying by corresponding window element
		data[i][0] = (*signal)[readIndex] * window[i]; 
		data[i][1] = 0.0; 
	    }
	    else // we have read beyond the signal, so zero-pad it
	    {
		data[i][0] = 0.0; 
		data[i][1] = 0.0; 
		bStop = 1;
	    }
	}
  
	// Perform the FFT on our chunk
	fftw_execute(plan_forward);
  
	/* Uncomment to see the raw-data output from the FFT calculation
	   std::cout << "Column: " << chunkPosition << std::endl;
	   for(i = 0 ; i < windowSize ; i++ ) {
	   fprintf( stdout, "fft_result[%d] = { %2.2f, %2.2f }\n",
	   i, fft_result[i][0], fft_result[i][1] );
	   }
	*/
  
  
	// Copy the first (windowSize/2 + 1) data points into your spectrogram.
	// We do this because the FFT output is mirrored about the nyquist 
	// frequency, so the second half of the data is redundant. This is how
	// Matlab's spectrogram routine works.
	for (i = 0; i < windowSize/2 + 1; i++)
	{
	    output_result[i] = fft_result[i][0];
	    output_result[i] = fft_result[i][1];
	}

	chunkPosition += hopSize;
  
    } // while loop ends here
 
    fftw_destroy_plan(plan_forward);
 
    fftw_free(data);
    fftw_free(fft_result);
    fftw_free(ifft_result);
}
