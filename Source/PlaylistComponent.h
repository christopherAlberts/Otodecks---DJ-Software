/*
  ==============================================================================

    PlaylistComponent.h
    Created: 1 Mar 2022 8:10:45pm
    Author:  chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include "Songs.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                            public TableListBoxModel,
                            public TextEditor::Listener,
                            public Button::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1,
                      DeckGUI* _deckGUI2,
                      DJAudioPlayer* _playerForParsingMetaData
                      );
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(Graphics&, 
                            int rowNumber, 
                            int width,
                            int height,
                            bool rowIsSelected) override;

    void paintCell(Graphics&,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber, 
                                        int columnId, 
                                        bool isRowSelected, 
                                        Component *existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

private:

    TableListBox tableComponent;
    std::vector<std::string> trackTitles;

    std::vector<Songs> tracks;

    juce::TextButton importButton{ "IMPORT TRACKS" };
    juce::TextEditor searchField;
    juce::TableListBox library;
    juce::TextButton addToPlayer1Button{ "ADD TO DECK 1" };
    juce::TextButton addToPlayer2Button{ "ADD TO DECK 2" };

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* playerForParsingMetaData;

    juce::String getLength(juce::URL audioURL);
    juce::String secondsToMinutes(double seconds);

    void importToLibrary();
    void searchLibrary(juce::String searchText);
    void saveLibrary();
    void loadLibrary();
    void deleteFromTracks(int id);
    bool isInTracks(juce::String fileNameWithoutExtension);
    int whereInSongs(juce::String searchText);
    void loadSongToPlayer(DeckGUI* deckGUI);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
