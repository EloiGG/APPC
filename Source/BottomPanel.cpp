/*
  ==============================================================================

    BottomPanel.cpp
    Created: 19 Jun 2021 1:08:30am
    Author:  Eloi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BottomPanel.h"

//==============================================================================
BottomPanel::BottomPanel()
{
    addAndMakeVisible(t);
    t.setFont(Font(15));
    t.setMultiLine(true);
    t.setReturnKeyStartsNewLine(true);
    Log::get().updateFunction = [this]() {t.setText(Log::get().getString(debuggingLevel)); };
}

BottomPanel::~BottomPanel()
{
}

void BottomPanel::paint (juce::Graphics& g)
{
    g.fillAll(lfColours::panelBackground);
    t.setText(Log::get().getString(debuggingLevel));
}

void BottomPanel::resized()
{
    t.setBounds(getLocalBounds());
}

TextEditor* BottomPanel::getTextEditor()
{
    return &t;
}