/*
  ==============================================================================

    RightPanel.cpp
    Created: 19 Jun 2021 1:07:30am
    Author:  Eloi

  ==============================================================================
*/

#include "TabbedComponent.h"

//==============================================================================
TabComponent::TabComponent() : TabbedComponent(TabbedButtonBar::Orientation::TabsAtTop)
{
    addTab(L"Général", lfColours::tabBackground, &genTab, true);
    addTab("Prix", lfColours::tabBackground, &priceTab, true);
}

TabComponent::~TabComponent()
{
}

void TabComponent::paint (juce::Graphics& g) 
{
    TabbedComponent::paint(g);
    //g.fillAll(lfColours::panelBackground);

    //g.setColour (juce::Colours::grey);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //g.setColour (juce::Colours::white);
    //g.setFont (14.0f);
    //g.drawText ("RightPanel", getLocalBounds(),
    //            juce::Justification::centred, true);   // draw some placeholder text
}

void TabComponent::resized()
{
    TabbedComponent::resized();
}

void TabComponent::updateParameters()
{
    genTab.updateAllParameters();
}
