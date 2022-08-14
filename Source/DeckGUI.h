/*
  ==============================================================================

    DeckGUI.h
    Created: 25 Feb 2022 6:09:05pm
    Author:  chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"



//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                public Button::Listener,
                public Slider::Listener,
                public FileDragAndDropTarget,
                public Timer

{
public:
    DeckGUI(int _id,
        DJAudioPlayer* player,
        AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& catcheToUse );

    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(Button*) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    void timerCallback() override;

private:

    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };

    Slider volSlider;
    Label volLabel;
    Slider  speedSlider;
    Label speedLabel;
    Slider posSlider;
    Label posLabel;
    
    



    int id;

    void loadFile(juce::URL audioURL);

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player;

    SharedResourcePointer< juce::TooltipWindow > sharedTooltip;
    friend class PlaylistComponent;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
