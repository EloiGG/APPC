/*
  ==============================================================================

    GeneralTab.h
    Created: 19 Jun 2021 10:42:19am
    Author:  Eloi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GridTool.h"
#include "LookAndFeel.h"
#include "InputComponent.h"
//==============================================================================
/*
*/
class GeneralTab  : public juce::Component
{
public:
    GeneralTab();
    ~GeneralTab() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setNumPrices(int newNumPrices);
    void setNumDigits(int newNumDigits);

    void updateAllParameters();

private:
    AlertWindow connectWindow, networkErrorWindow, networkSuccessWindow;
    TextButton connectButton, loadConfigButton;
    GridTool grid;
    InputComponent nPrices, nDigits;
    CheckBox lineControl, resetLine;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneralTab)
};
