/*
  ==============================================================================

    LeftPanel.h
    Created: 19 Jun 2021 1:07:21am
    Author:  Eloi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"

//==============================================================================
/*
*/
class LeftPanel  : public juce::Component
{
public:
    LeftPanel();
    ~LeftPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LeftPanel)
};
