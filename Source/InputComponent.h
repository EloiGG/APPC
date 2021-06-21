/*
  ==============================================================================

    InputComponent.h
    Created: 21 Jun 2021 2:24:45pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class InputComponent  : public juce::Component
{
public:
    InputComponent();
    ~InputComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputComponent)
};
