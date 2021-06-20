/*
  ==============================================================================

    PriceTab.cpp
    Created: 19 Jun 2021 10:42:31am
    Author:  Eloi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PricesTab.h"

//==============================================================================
PricesTab::PricesTab() : TabbedComponent(TabbedButtonBar::Orientation::TabsAtTop)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

PricesTab::~PricesTab()
{
}

void PricesTab::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PriceTab", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PricesTab::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
