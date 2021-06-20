/*
  ==============================================================================

    TopPanel.h
    Created: 19 Jun 2021 1:06:59am
    Author:  Eloi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"

//==============================================================================
/*
*/
class TopPanel  : public juce::Component
{
public:
    TopPanel();
    ~TopPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopPanel)
};
