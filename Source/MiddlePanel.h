/*
  ==============================================================================

    MiddlePanel.h
    Created: 19 Jun 2021 1:07:43am
    Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PriceDisplayComponent.h"
#include "LookAndFeel.h"
#include "Core.h"
#include "DigitComponent.h"
#include "PriceComponent.h"
#include "GridTool.h"
#include "Buttons.h"
#include "Keyboard.h"
#include "Disabled.h"


//==============================================================================
/*
*/

class HighLight : public juce::Component
{
public:
    HighLight();
    void resize(unsigned int newX, unsigned int newY);
    void highlightRow(int rowIndex);
    void highlightColumn(int columnIndex);
    void highlightAll();
    void stopHighlighting();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::Rectangle<int> hlRect;
    GridTool grid;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HighLight)
};

class MiddlePanel  : public juce::Component
{
public:
    MiddlePanel();
    ~MiddlePanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void updatePrices(TextUpdateOrigin whocalled, unsigned int index);
    void updateVisualization();
    void mouseExit(const MouseEvent&) override;
    void mouseEnter(const MouseEvent&) override;

private:
    HighLight highlights;
    GridTool topGrid, leftGrid;
    PriceDisplayComponent prices;
    PriceComponent pricecomp;
    DigitEditor topDigits[Core::MAX_DIGITS];
    DigitEditor leftDigits[Core::MAX_PRICES];
    DigitEditor cornerDigit;
    Buttons buttons;
    Label name;
    juce::Rectangle<int> nameArea, openConfigArea, keyboardArea;
    Keyboard kb;
    ShapeButton openConfig;
    Disabled disable;
    int nPrices, nDigits;
    ApplicationCommandManager commandManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiddlePanel)
};
