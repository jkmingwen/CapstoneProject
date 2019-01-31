#include "ofMain.h"
#include "ofApp.h"
#include "fftw3.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context
	ofRunApp(new ofApp());

}
