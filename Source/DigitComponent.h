/*
  ==============================================================================

    DigitComponent.h
    Created: 17 Jun 2021 11:04:03am
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"

//==============================================================================
/*
*/
class DigitEditor : public Label
{
public:
    DigitEditor();

    void paint(juce::Graphics&) override;
    void setDigit(String& newDigit);
    virtual void mouseDown(const MouseEvent&) override;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DigitEditor)
};

class DigitComponent  : public juce::Component
{
public:
    DigitComponent();
    ~DigitComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DigitEditor digit;
    Font font;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DigitComponent)
};
