/*
  ==============================================================================

    TopPanel.cpp
    Created: 19 Jun 2021 1:06:59am
    Author:  Eloi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TopPanel.h"

//==============================================================================
TopPanel::TopPanel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

TopPanel::~TopPanel()
{
}

void TopPanel::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(lfColours::panelBackground);
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("TopPanel", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void TopPanel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
