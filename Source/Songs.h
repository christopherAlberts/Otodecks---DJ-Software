/*
  ==============================================================================

    Songs.h
    Created: 5 Mar 2022 1:50:31pm
    Author:  chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Songs
{
public:

    Songs(juce::File _file);
    juce::File file;
    juce::URL URL;
    juce::String title;
    juce::String length;
    /**objects are compared by title*/
    bool operator==(const juce::String& other) const;

};
