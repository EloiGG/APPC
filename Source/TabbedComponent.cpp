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
    addTab(CharPointer_UTF8("Général"), lfColours::tabBackground, &genTab, true);
    addTab("Prix", lfColours::tabBackground, &priceTab, true);
    addTab(CharPointer_UTF8("Dépannage"), lfColours::tabBackground, &debugTab, true);
}

TabComponent::~TabComponent()
{
}

void TabComponent::paint (juce::Graphics& g) 
{
    TabbedComponent::paint(g);
}

void TabComponent::resized()
{
    TabbedComponent::resized();
}

void TabComponent::updateParameters()
{
    genTab.updateAllParameters();
}
