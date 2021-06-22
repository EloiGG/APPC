/*
  ==============================================================================

    RightPanel.h
    Created: 21 Jun 2021 12:36:05pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TabbedComponent.h"
#include "Buttons.h"

//==============================================================================
/*
*/
class RightPanel  : public juce::Component
{
public:
    RightPanel();
    ~RightPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void updateParameters();

private:
    TabComponent tabs;
    Buttons buttons;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RightPanel)
};
