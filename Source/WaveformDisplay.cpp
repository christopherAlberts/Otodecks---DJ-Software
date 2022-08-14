/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 28 Feb 2022 7:16:42pm
    Author:  chris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(int _id, 
                                AudioFormatManager& formatManagerToUse,
                                AudioThumbnailCache& cacheToUse) :
                                audioThumb(1000, formatManagerToUse, cacheToUse),
                                fileLoaded(false),
                                position(0),
                                id(_id)
{
    
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawText("Deck: " + std::to_string(id), getLocalBounds(),
        juce::Justification::centredTop, true);

    g.setColour (juce::Colours::aquamarine);
    if (fileLoaded)
    {
        g.setFont(15.0f);
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            1.0f
            );
        g.setColour(Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
        g.setColour(juce::Colours::white);
        g.drawText(fileName, getLocalBounds(),
            juce::Justification::centredBottom, true);
    }

    else
    {
        g.setFont(20.0f);
        g.drawText("Please load audio file...", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }

}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(URL audioURL)
{

    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded)
    {
        DBG("WaveformDisplay::loadURL file loaded");
        // std::cout << "WaveformDisplay::loadURL file loaded!" << std::endl;
        fileName = audioURL.getFileName();
        repaint();
    }
    else
    {
        DBG("WaveformDisplay::loadURL file NOT loaded");
        // std::cout << "WaveformDisplay::loadURL file NOT loaded" << std::endl;
    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    DBG("wfd: change recieved!");
    // std::cout << "wfd: change recieved!" << std::endl;

    repaint();

}

void WaveformDisplay::setPositionRelative(double pos)
{
    if( pos != position && !isnan(pos));
    {
        position = pos;
        repaint();

    }
    
    
}