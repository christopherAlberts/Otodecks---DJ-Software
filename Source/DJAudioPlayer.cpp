/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 25 Feb 2022 1:21:17pm
    Author:  chris

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
    : formatManager(_formatManager)
{

}
DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{

    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        // transportSource.start();
    }
}
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        DBG("DJAudioPlyer::setGain gain should be between 0 - 1.");
        // std::cout << "DJAudioPlyer::setGain gain should be between 0 - 1." << std::endl;
    }
    else
    {
        transportSource.setGain(gain);
    }
    
}
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0.1 || ratio > 100.0)
    {
        DBG("DJAudioPlyer::setSpeed ratio should be between 0 - 100.");
        // std::cout << "DJAudioPlyer::setSpeed ratio should be between 0 - 1." << std::endl;
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0.1 || pos > 1.0)
    {
        DBG("DJAudioPlyer::setPositionRelative pos should be between 0 - 1.");
        // std::cout << "DJAudioPlyer::setPositionRelative pos should be between 0 - 1." << std::endl;
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start()
{
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{   
    DBG("transportSource.getLengthInSeconds(): " << transportSource.getLengthInSeconds());
    DBG("transportSource.getCurrentPosition(): " << transportSource.getCurrentPosition());
    if (transportSource.getLengthInSeconds() == 0 || isnan(transportSource.getLengthInSeconds()))
    {
        return transportSource.getCurrentPosition() / 1;
    }
    else if (transportSource.getLengthInSeconds() > 0)
    {
        return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    }
    DBG("transportSource.getLengthInSeconds(): " << transportSource.getLengthInSeconds());
    DBG("transportSource.getCurrentPosition(): " << transportSource.getCurrentPosition());
    
}

double DJAudioPlayer::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}