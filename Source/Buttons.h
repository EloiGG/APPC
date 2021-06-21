/*
  ==============================================================================

    Buttons.h
    Created: 21 Jun 2021 12:37:46pm
    Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"
#include "GridTool.h"
#include "Core.h"

//==============================================================================
/*
*/
class Buttons  : public juce::Component
{
public:
    Buttons();
    ~Buttons() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    TextButton send, stop, prixP, prixM, digitP, digitM;
    GridTool grid;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Buttons)
};
