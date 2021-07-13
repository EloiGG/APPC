/*
  ==============================================================================

    RightPanel.cpp
    Created: 21 Jun 2021 12:36:05pm
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RightPanel.h"

//==============================================================================
RightPanel::RightPanel()
{
    addAndMakeVisible(tabs);
    //addAndMakeVisible(buttons);
}

RightPanel::~RightPanel()
{
}

void RightPanel::paint (juce::Graphics& g)
{
    g.fillAll(lfColours::panelBackground);
}

void RightPanel::resized()
{
    float separtion = 0.22f;
    auto r = getLocalBounds();
    //buttons.setBounds(r.removeFromBottom(separtion * getHeight()));
    tabs.setBounds(r);
}

void RightPanel::updateParameters()
{
    tabs.updateParameters();
    buttons.updateVizualisation();
}
