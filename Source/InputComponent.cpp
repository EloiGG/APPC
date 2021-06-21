/*
  ==============================================================================

    InputComponent.cpp
    Created: 21 Jun 2021 2:24:45pm
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "InputComponent.h"

//==============================================================================
InputComponent::InputComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

InputComponent::~InputComponent()
{
}

void InputComponent::paint (juce::Graphics& g)
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
    g.drawText ("InputComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void InputComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
