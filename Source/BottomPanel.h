/*
  ==============================================================================

    BottomPanel.h
    Created: 19 Jun 2021 1:08:30am
    Author:  Eloi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"

//==============================================================================
/*
*/
class BottomPanel  : public juce::Component
{
public:
    BottomPanel();
    ~BottomPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottomPanel)
};
