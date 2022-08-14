/*
  ==============================================================================

    Songs.cpp
    Created: 5 Mar 2022 1:50:31pm
    Author:  chris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Songs.h"
#include <filesystem>

//==============================================================================
Songs::Songs(juce::File _file) : file(_file),
                                title(_file.getFileNameWithoutExtension()),
                                URL(juce::URL{ _file })
{

    DBG("Created a new song with the following title: " << title);
}

bool Songs::operator==(const juce::String& other) const
{
    return title == other;
}


