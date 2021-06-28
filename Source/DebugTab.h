/*
  ==============================================================================

    DebugTab.h
    Created: 28 Jun 2021 3:24:13pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"
#include "JSON.h"
#include "GridTool.h"
#include "Core.h"
#include "InputComponent.h"

//==============================================================================
/*
*/
class DebugTab  : public juce::Component
{
public:
    DebugTab();
    ~DebugTab() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    TextButton loadSequence;
    GridTool grid;
    FileChooser fileSelector;
    CheckBox playSequenceCB;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DebugTab)
};
