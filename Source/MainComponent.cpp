#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    freqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    freqSlider.setNumDecimalPlacesToDisplay(2);
    freqSlider.setRange(50, 500);
    freqSlider.setTextValueSuffix(" Hz");
    freqSlider.addListener(this);
    freqSlider.setValue(200);
    addAndMakeVisible(freqSlider);
    freqLabel.setText("Frequency", juce::dontSendNotification);
    freqLabel.attachToComponent(&freqSlider, true);


    ampSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    ampSlider.setRange(0.0, 1.0);
    ampSlider.addListener(this);
    ampSlider.setValue(0.0);
    addAndMakeVisible(ampSlider);
    ampLabel.setText("Amplitude", juce::dontSendNotification);
    ampLabel.attachToComponent(&ampSlider, true);


    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    frequency = freqSlider.getValue();
    phase = 0;
    wSize = 1024;
    increment = frequency * wSize / currentSampleRate;   //cuantas muestras hay que incrementar para consguir una velocidad de 440 Hz
    amplitude = ampSlider.getValue();
    currentSampleRate = sampleRate;

    //un ciclo de una onda seno
    for (int i = 0; i < wSize; i++) 
    {
        // se normalizan los valores del ciclo de una onda seno en 1024 muestras
        waveTable.insert(i, sin(2.0 * juce::double_Pi * i / wSize));
    }
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    float* const leftSpeaker = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);   // punteros que apuntan a la priemra muesta del buffer derecho e izquierdo
    float* const rightSpeaker = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (int sample = 0; sample < bufferToFill.numSamples; sample++)
    {

        // rellenamos cada buffer con muestras del wavetable
        leftSpeaker[sample] = waveTable[(int)phase] * amplitude;
        rightSpeaker[sample] = waveTable[(int)phase] * amplitude;
        updateFrequency();     
        
    }
}

void MainComponent::updateFrequency()
{
    increment = frequency * wSize / currentSampleRate;
    phase = fmod((phase + increment), wSize); //con esta funcion se evita que la fase supere las 1024 muestras, al ser la fase un numero no entero
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    const int labelSpace = 100;
    freqSlider.setBounds(labelSpace, 20, getWidth() - 100, 20);
    ampSlider.setBounds(labelSpace, 50, getWidth() - 100, 20);

}
