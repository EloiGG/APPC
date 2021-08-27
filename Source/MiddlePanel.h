/*
  ==============================================================================

    MiddlePanel.h
    Created: 19 Jun 2021 1:07:43am
    Author:  Eloi GUIHARD-GOUJON

    Contient les éléments visuel du panneau principal du logiciel

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

/// <summary>
/// Classe permettant de surligner certaines parties des prix
/// </summary>
class HighLight : public juce::Component
{
public:
    HighLight();

    // Change le nombre de colonnes et de lignes
    void resize(unsigned int newX, unsigned int newY);

    // Surligne la ligne spécifiée
    void highlightRow(int rowIndex);

    // Surligne la colonne spécifiée
    void highlightColumn(int columnIndex);

    // Surligne tout le component
    void highlightAll();

    // Arrête tout surlignage en cours
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

    // Met à jour tous les prix affichés
    void updatePrices(TextUpdateOrigin whocalled, unsigned int index);

    // Met à jour tout le visuel
    void updateVisualization();

    // Fonction appelée lorsque la souris sort du component
    void mouseExit(const MouseEvent&) override;

    // Fonction appelée lorsque la souris entre dans le component
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
    ShapeButton openConfig;
    Disabled disable;
    int nPrices, nDigits;
    ApplicationCommandManager commandManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiddlePanel)
};
