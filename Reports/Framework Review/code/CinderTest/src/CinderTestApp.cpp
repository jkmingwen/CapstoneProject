#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/audio/audio.h"
#include "AudioDrawUtils.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CinderTestApp : public App {
public:
    void setup() override;
    void mouseDown( MouseEvent event ) override;
    void update() override;
    void draw() override;

    // The 'ref's are typedefs of pointers to their classes
    audio::InputDeviceNodeRef mInputDeviceNode;
    audio::MonitorNodeRef mMonitorNode;
};

void CinderTestApp::setup()
{
    // Create master Context
    auto ctx = audio::Context::master(); // this faces hardware
    // Create InputDeviceNode (has to be done using Context method)
    mInputDeviceNode = ctx->createInputDeviceNode();
    // Create MonitorNode with 1024 samples in window
    auto monitorFormat = audio::MonitorNode::Format().windowSize(256);
    mMonitorNode = ctx->makeNode(new audio::MonitorNode(monitorFormat));
    // feed input node to monitor node
    mInputDeviceNode >> mMonitorNode;
    
    // enable nodes to process audio
    mInputDeviceNode->enable();
    ctx->enable();
    // set window name to audio device name
    getWindow()->setTitle(mInputDeviceNode->getDevice()->getName());
}

void CinderTestApp::mouseDown( MouseEvent event )
{
}

void CinderTestApp::update()
{
}

void CinderTestApp::draw()
{
    gl::clear();
    const audio::Buffer &buffer = mMonitorNode->getBuffer();
    Rectf scopeRect(getWindowWidth() - getWindowWidth()/4,
                    getWindowHeight()/5,
                    0 + getWindowWidth()/4,
                    getWindowHeight() - getWindowHeight()/5);
    drawAudioBuffer(buffer, scopeRect, false, ci::ColorA(1, 1, 0.3f, 0.5));
}

CINDER_APP( CinderTestApp, RendererGl )
