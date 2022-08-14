 /*
  ==============================================================================

    DJAudioPlayer.h
    Created: 25 Feb 2022 1:21:17pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource {
    public:

        DJAudioPlayer(AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        void loadURL(URL audioURL);
        void setGain(double gain);
        void setSpeed(double ratio);
        void setPosition(double posInSecs);
        void setPositionRelative(double pos);
        /**Gets the length of transport source in seconds*/
        double getLengthInSeconds();
        void start();
        void stop();

        /** Get the relative Position of the Play Head*/
        double getPositionRelative();

    private:

        AudioFormatManager& formatManager;
        std::unique_ptr<AudioFormatReaderSource> readerSource;
        AudioTransportSource transportSource;
        ResamplingAudioSource resampleSource{&transportSource, false, 2};

};
