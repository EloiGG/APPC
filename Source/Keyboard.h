/*
  ==============================================================================

    Keyboard.h
    Created: 13 Jul 2021 2:08:34pm
    Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Core.h"
#include "GridTool.h"
#include "LookAndFeel.h"
#include "SpecialLabel.h"

//==============================================================================
/*
*/

class Keyboard  : public juce::Component
{
public:
    static constexpr size_t NUM_ROWS = 4;
    static constexpr size_t NUM_COLUMNS = 4;
    static constexpr size_t NUM_KEYS = NUM_ROWS * NUM_COLUMNS;
public:
    Keyboard();
    ~Keyboard() override;
    void resetAndShow(unsigned int maxNumberOfCharacters = Core::MAX_DIGITS);
    void paint (juce::Graphics&) override;
    void resized() override;
    String getOutput() { return output; }
private:
    unsigned int currentMaxNumChar;
    String output;
    KeyboardLookAndFeel lf;
    TextButton key[NUM_KEYS];
    GridTool grid;
    PriceEditorLabel label;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Keyboard)
};
