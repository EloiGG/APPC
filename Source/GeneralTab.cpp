/*
  ==============================================================================

    GeneralTab.cpp
    Created: 19 Jun 2021 10:42:19am
    Author:  Eloi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GeneralTab.h"

//==============================================================================
GeneralTab::GeneralTab() : grid(4, 8), test("test"), l("idk", "APPA")
{
    addAndMakeVisible(grid);
    addAndMakeVisible(test);
    addAndMakeVisible(l);
    l.attachToComponent(&test, true);
}

GeneralTab::~GeneralTab()
{
}

void GeneralTab::paint (juce::Graphics& g)
{
    g.fillAll(lfColours::tabBackground);
}

void GeneralTab::resized()
{
    grid.setBounds(getLocalBounds());
    test.setBounds(grid.getRectangle(2, 2, 3, 3));
    l.setBounds(grid.getRectangle(1, 2, 2, 3));
}
