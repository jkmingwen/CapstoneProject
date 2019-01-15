/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, private Timer // Timer class to make repeated callbacks to virtual class
{
public:
    //==============================================================================
    MainComponent()
    {
        setOpaque(true); // we don't draw layers beneath component (to optimise drawing)
        startTimerHz(60); // frame refresh rate set at 60Hz
        setSize (800, 600);
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 0);
    }

    ~MainComponent()
    {
        // This shuts down the audio device and clears the audio source.
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override {}

    void releaseResources() override {}
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        if (bufferToFill.buffer->getNumChannels() > 0) // execute if we have channels open
        {
            // we get a pointer to the sample taken in
            auto *inputData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample); // pick first channel (0), point to first sample
            for (auto i = 0; i < bufferToFill.numSamples; ++i) // iterate through buffer's samples. why "++i" though?
            {
                pushNextSample(inputData[i]); // push pointer to sample into array
            }
        }
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        
        g.setColour(Colours::deeppink);
        drawFrame(g);
    }
    
    void timerCallback() override
    {
        drawNextBuffer();
        nextBufferReady = false;
        repaint();
    }

    void resized() override {}
    
    void pushNextSample (float sample) noexcept
    {
        if (bufferIndex == bufferSize) // replace first oldest sample
        {
            if (!nextBufferReady)
            {
                zeromem(visData, sizeof(visData)); // empty visualiser buffer data
                memcpy(visData, buffer, sizeof(buffer));
                nextBufferReady = true;
            }
            bufferIndex = 0;
        }
        buffer[bufferIndex] = sample; // write next sample to buffer
        bufferIndex++;
    }
    
    void drawNextBuffer()
    {
        auto minAmp = -100.0f;
        auto maxAmp = 0.0f;
        
        for (int i = 0; i < bufferSize; i++)
        {
            auto sampleAmp = Decibels::gainToDecibels(buffer[bufferIndex]) - Decibels::gainToDecibels((float) bufferSize);
            auto scaledAmp = jmap (jlimit (minAmp, maxAmp, sampleAmp),
                                   minAmp, maxAmp, 0.0f, 1.0f); // maps sample gain to value between 0.0 and 1.0
            scaledAmp = jmap (sampleAmp, minAmp, maxAmp);
            visData[i] = scaledAmp;
        }
    }

    void drawFrame (Graphics &g)
    {
        auto xMin = 0;
        auto xMax = getLocalBounds().getWidth();
        auto yMin = getLocalBounds().getHeight() / 2;
        auto yMax = 0.0;
        for (int i = 0; i < bufferSize; ++i)
        {
//            g.drawEllipse((float)jmap(i, 0, bufferSize - 1, xMin, xMax),
//                          jmap(visData[i], 0.0f, 1.0f, (float) yMin, (float) yMax),
//                          10, 10, 1.0f);
            // to draw sound wave, draw line from previous sample to next sample
            // map width and height of sound wave using jmap function
            g.drawLine((float) jmap(i - 1, 0, bufferSize - 1, xMin, xMax),
                       jmap(visData[i - 1], 0.0f, 1.0f, (float) yMin, (float) yMax),
                       (float) jmap(i, 0, bufferSize - 1, xMin, xMax),
                       jmap(visData[i], 0.0f, 1.0f, (float) yMin, (float) yMax),
                       5.0f);
        }
    }

    enum
    {
        bufferSize = 512
    };

private:
    float buffer[bufferSize]; // hold two visData's worth of samples
    int bufferIndex = 0;
    bool nextBufferReady = false;
    float visData[bufferSize];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
