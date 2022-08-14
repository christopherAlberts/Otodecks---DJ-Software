/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 1 Mar 2022 8:10:45pm
    Author:  chris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                    DeckGUI* _deckGUI2,
                                    DJAudioPlayer* _playerForParsingMetaData
                                    ) : deckGUI1(_deckGUI1),
                                    deckGUI2(_deckGUI2),
                                    playerForParsingMetaData(_playerForParsingMetaData)
{

    // add components
    addAndMakeVisible(importButton);
    addAndMakeVisible(searchField);
    addAndMakeVisible(library);
    addAndMakeVisible(addToPlayer1Button);
    addAndMakeVisible(addToPlayer2Button);

    // attach listeners
    importButton.addListener(this);
    searchField.addListener(this);
    addToPlayer1Button.addListener(this);
    addToPlayer2Button.addListener(this);

    // searchField configuration
    searchField.setTextToShowWhenEmpty("Search Songs... (Press enter to submit)",juce::Colours::orange);
    searchField.onReturnKey = [this] { searchLibrary(searchField.getText()); };


    // setup table and load library from file
    library.getHeader().addColumn("Songs", 1, 1);
    library.getHeader().addColumn("Length", 2, 1);
    library.getHeader().addColumn("", 3, 1);
    library.setModel(this);
    loadLibrary();
}

PlaylistComponent::~PlaylistComponent()
{

    saveLibrary();
}

void PlaylistComponent::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setColour (juce::Colours::lightskyblue);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::grey);
    g.setFont (14.0f);

    importButton.setColour(TextButton::ColourIds::buttonColourId, Colours::purple);
    addToPlayer1Button.setColour(TextButton::ColourIds::buttonColourId, Colours::mediumslateblue);
    addToPlayer2Button.setColour(TextButton::ColourIds::buttonColourId, Colours::mediumslateblue);

}

void PlaylistComponent::resized()
{

    // components that your component contains...
    tableComponent.setBounds(0, 0, getWidth() / 2, getHeight() / 16);

    importButton.setBounds(0, 0, getWidth() / 2, 2 * getHeight() / 16);
    searchField.setBounds(getWidth() / 2, 0 , getWidth() / 2, 2 * getHeight() / 16 );
    
    addToPlayer1Button.setBounds(0, 2 * getHeight() / 16, getWidth() / 2, 2 * getHeight() / 16);
    addToPlayer2Button.setBounds(getWidth() / 2, 2 * getHeight() / 16, getWidth() / 2, 2 * getHeight() / 16);

    library.setBounds(0, 4 * getHeight() / 16, getWidth(), 12 * getHeight() / 16);

    //set columns
    library.getHeader().setColumnWidth(1, 12.8 * getWidth() / 20);
    library.getHeader().setColumnWidth(2, 5 * getWidth() / 20);
    library.getHeader().setColumnWidth(3, 2 * getWidth() / 20);

}

int PlaylistComponent::getNumRows()
{

    return tracks.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g,
                                        int rowNumber,
                                        int width,
                                        int height,
                                        bool rowIsSelected
                                        )
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::aquamarine);
    }
    else
    {
        g.fillAll(Colours::skyblue);
    }
}

void PlaylistComponent::paintCell(Graphics& g,
                                    int rowNumber,
                                    int columnId,
                                    int width,
                                    int height,
                                    bool rowIsSelected
                                    )
{

    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
            g.drawText(tracks[rowNumber].title,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centredLeft,
                true
            );
        }
        if (columnId == 2)
        {
            g.drawText(tracks[rowNumber].length,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centred,
                true
            );
        }
    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                        int columnId,
                                                        bool isRowSelected,
                                                        Component *existingComponentToUpdate
                                                        )
{

   
    if (columnId == 3)
    {
        DBG("PlaylistComponent::refreshComponentForCell col3: ");

        if (existingComponentToUpdate == nullptr)
        {
   
            juce::TextButton* btn = new juce::TextButton{ "Remove" };
            juce::String id{ std::to_string(rowNumber) };

            btn->setColour(TextButton::ColourIds::buttonColourId, Colours::coral);

            DBG("PlaylistComponent::refreshComponentForCell id: " << id);
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}


void PlaylistComponent::buttonClicked(Button* button)
{
    DBG("PlaylistComponent::buttonClicked! 1: " << button->getComponentID().toStdString());
    if (button->getComponentID().toStdString() == "")
    {
        DBG("PlaylistComponent::buttonClicked == ''");
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());

    }


    if (button == &importButton)
    {
        DBG("Load button clicked");
        importToLibrary();
        library.updateContent();
    }
    else if (button == &addToPlayer1Button)
    {
        DBG("Add to Player 1 clicked");
        loadSongToPlayer(deckGUI1);
    }
    else if (button == &addToPlayer2Button)
    {
        DBG("Add to Player 2 clicked");
        loadSongToPlayer(deckGUI2);
    }
    else
    {

        if (button->getComponentID().toStdString() == "")
        {
            DBG("PlaylistComponent::buttonClicked == ''");
        }
        else
        {
            int id = std::stoi(button->getComponentID().toStdString());
            DBG(tracks[id].title + " removed from Library");
            deleteFromTracks(id);
            library.updateContent();
        }


    }
}












void PlaylistComponent::loadSongToPlayer(DeckGUI* deckGUI)
{
    int selectedRow{ library.getSelectedRow() };
    if (selectedRow != -1)
    {
        DBG("Adding: " << tracks[selectedRow].title << " to Player");
        deckGUI->loadFile(tracks[selectedRow].URL);
    }
    else
    {
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
            "There is no song to add to deck:", "Please select a track to add", "OK",
            nullptr
        );
    }
}

void PlaylistComponent::importToLibrary()
{
    DBG("PlaylistComponent::importToLibrary called");

    //initialize file chooser
    juce::FileChooser chooser{ "Please Select files" };
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const juce::File& file : chooser.getResults())
        {
            juce::String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
            if (!isInTracks(fileNameWithoutExtension)) // if file is not already loaded
            {
                Songs newTrack{ file };
                juce::URL audioURL{ file };
                newTrack.length = getLength(audioURL);
                tracks.push_back(newTrack);
                DBG("loaded file: " << newTrack.title);
            }
            else // display info message
            {
                juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
                    "Load information:",
                    fileNameWithoutExtension + " already loaded",
                    "OK",
                    nullptr
                );
            }
        }
    }
}

bool PlaylistComponent::isInTracks(juce::String fileNameWithoutExtension)
{
    return (std::find(tracks.begin(), tracks.end(), fileNameWithoutExtension) != tracks.end());
}

void PlaylistComponent::deleteFromTracks(int id)
{   
    DBG("PlaylistComponent::deleteFromTracks: id:" + id);
    tracks.erase(tracks.begin() + id);

}

juce::String PlaylistComponent::getLength(juce::URL audioURL)
{
    playerForParsingMetaData->loadURL(audioURL);
    double seconds{ playerForParsingMetaData->getLengthInSeconds() };
    juce::String minutes{ secondsToMinutes(seconds) };
    return minutes;
}

juce::String PlaylistComponent::secondsToMinutes(double seconds)
{
    //find seconds and minutes and make into string
    int secondsRounded{ int(std::round(seconds)) };
    juce::String min{ std::to_string(secondsRounded / 60) };
    juce::String sec{ std::to_string(secondsRounded % 60) };

    if (sec.length() < 2) // if seconds is 1 digit or less
    {
        //add '0' to seconds until seconds is length 2
        sec = sec.paddedLeft('0', 2);
    }
    return juce::String{ min + ":" + sec };
}

void PlaylistComponent::searchLibrary(juce::String searchText)
{
    DBG("Searching library for: " << searchText);
    if (searchText != "")
    {
        int rowNumber = whereInSongs(searchText);
        library.selectRow(rowNumber);
    }
    else
    {
        library.deselectAllRows();
    }
}

int PlaylistComponent::whereInSongs(juce::String searchText)
{
    // finds index where track title contains searchText
    auto it = find_if(tracks.begin(), tracks.end(),
        [&searchText](const Songs& obj) {return obj.title.contains(searchText); });
    int i = -1;

    if (it != tracks.end())
    {
        i = std::distance(tracks.begin(), it);
    }

    return i;
}

void PlaylistComponent::saveLibrary()
{
    // create .csv to save library
    std::ofstream myLibrary("my-library.csv");

    // save library to file
    for (Songs& t : tracks)
    {
        myLibrary << t.file.getFullPathName() << "," << t.length << "\n";
    }
}

void PlaylistComponent::loadLibrary()
{
    // create input stream from saved library
    std::ifstream myLibrary("my-library.csv");
    std::string filePath;
    std::string length;

    // Read data, line by line
    if (myLibrary.is_open())
    {
        while (getline(myLibrary, filePath, ',')) {
            juce::File file{ filePath };
            Songs newTrack{ file };

            getline(myLibrary, length);
            newTrack.length = length;
            tracks.push_back(newTrack);
        }
    }
    myLibrary.close();
}







