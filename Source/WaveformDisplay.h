/*
  ==============================================================================

    WaveformDisplay.h
    Created: 28 Feb 2022 7:16:42pm
    Author:  chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
                         public ChangeListener
{
public:
    WaveformDisplay(int _id, AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback (ChangeBroadcaster* source) override;

    void loadURL(URL audioURL);

    /**Set the relative position of the playhead*/
    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;
    String fileName;
    int id;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
