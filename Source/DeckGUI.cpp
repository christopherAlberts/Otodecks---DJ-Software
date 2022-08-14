/*
  ==============================================================================

    DeckGUI.cpp
    Created: 25 Feb 2022 6:09:05pm
    Author:  chris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"


//==============================================================================
DeckGUI::DeckGUI(int _id,
                    DJAudioPlayer* _player,
                    AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse)
                    : player(_player),
                    id(_id),
                    waveformDisplay(id, formatManagerToUse, cacheToUse)
                    
                
                    
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(posLabel);

    addAndMakeVisible(waveformDisplay);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    posSlider.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);


    posSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    volSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);

    //posSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 100, 25);
    //volSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    //speedSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    

    getLookAndFeel().setColour(Slider::thumbColourId, Colours::purple);
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colours::deeppink);
    getLookAndFeel().setColour(Slider::trackColourId, Colours::deeppink);

    posSlider.setRange(0.1, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(2);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
        false,
        0,
        posSlider.getTextBoxHeight()
    );

    //posLabel.setText("Speed", juce::dontSendNotification);
    //posLabel.attachToComponent(&speedSlider, true);

    
    double startVol = 0.5; //set starting volume value 
    volSlider.setRange(0.1, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(2);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
        false,
        50,
        volSlider.getTextBoxHeight()
    );
    volSlider.setValue(startVol);
    volSlider.setSkewFactorFromMidPoint(startVol);

    double startSpeed = 1; //set starting speed value
    speedSlider.setRange(0.1, 10.0);   
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
        false,
        50,
        speedSlider.getTextBoxHeight()
    );
    speedSlider.setValue(startSpeed);
    speedSlider.setSkewFactorFromMidPoint(startSpeed);

    startTimer(500);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{

    g.fillAll(Colours::lightskyblue);
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::purple);
    g.setFont (18.0f);
    
    playButton.setColour(TextButton::ColourIds::buttonColourId, Colours::mediumslateblue);
    stopButton.setColour(TextButton::ColourIds::buttonColourId, Colours::mediumslateblue);
    loadButton.setColour(TextButton::ColourIds::buttonColourId, Colours::mediumslateblue);
    
    // Indicate which Slider is for "Vol" and which Slider is for "Speed".
    g.drawText ("Vol    Speed", getLocalBounds(),juce::Justification::centred, true);   
  
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 8;
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);

    posSlider.setBounds(0, rowH * 2, getWidth() , rowH );

    volSlider.setBounds(0, rowH * 3, getWidth() /2, rowH * 4);
    speedSlider.setBounds(getWidth() / 2, rowH * 3, getWidth() / 2, rowH * 4);
    
    playButton.setBounds(0, rowH * 7, getWidth() / 3, rowH * 1);
    stopButton.setBounds(getWidth() / 3, rowH * 7, getWidth() / 3, rowH * 1);
    loadButton.setBounds(2 * getWidth() / 3, rowH * 7, getWidth() / 3, rowH * 1);

}


void DeckGUI::buttonClicked(Button* button)
{

    if (button == &playButton) 
    {
        DBG("PLAY Button was clicked ");
        // std::cout << "PLAY Button was clicked " << std::endl;
        player->start();
    }
    if (button == &stopButton)
    {
        DBG("STOP Button was clicked ");
        // std::cout << "STOP Button was clicked " << std::endl;
        player->stop();
    }
    if (button == &loadButton)
    {
        DBG("LOAD Button was clicked ");
        // std::cout << "LOAD Button was clicked " << std::endl;

        FileChooser chooser{"Select a file ..."};
        
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(URL{ chooser.getResult() });
            waveformDisplay.loadURL(URL{ chooser.getResult() });
        }
    }

}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        DBG("volslider moved! " << slider->getValue() );
        // std::cout << "volslider moved! " << slider->getvalue() << std::endl;

        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        DBG("speedslider moved! " << slider->getValue());
        // std::cout << "speedslider moved! " << slider->getvalue() << std::endl;

        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        DBG("posslider moved! " << slider->getValue());
        // std::cout << "posslider moved! " << slider->getvalue() << std::endl;

        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag " );
    // std::cout << "DeckGUI::isInterestedInFileDrag  " << slider->getvalue() << std::endl;
    return true;
}
void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    DBG("DeckGUI::filesDropped " );
    // std::cout << "DeckGUI::filesDropped  " << slider->getvalue() << std::endl;
    if (files.size() == 1)
    {
        player->loadURL(URL{File{files[0]}});
    }

}

void DeckGUI::timerCallback()
{
    DBG("DeckGUI::timerCallback ");
    // std::cout << "DeckGUI::timerCallback  " << slider->getvalue() << std::endl;
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

void DeckGUI::loadFile(juce::URL audioURL)
{
    DBG("DeckGUI::loadFile called");
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}