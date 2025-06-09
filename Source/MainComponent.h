#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::Slider::Listener
{
public:

    void sliderValueChanged(juce::Slider* slider)
    {
        if (slider == &freqSlider)
        {
            frequency = freqSlider.getValue();
           
        }
        else if (slider == &ampSlider) {
            amplitude = ampSlider.getValue();
        }
    }

    void updateFrequency();
    
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    juce::Array<float> waveTable;
    double wSize;
    double frequency;
    double phase;
    double increment;
    double amplitude;
    double currentSampleRate;

    juce::Slider freqSlider, ampSlider;
    juce::Label freqLabel, ampLabel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
