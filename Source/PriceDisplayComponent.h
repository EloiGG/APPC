/*
  ==============================================================================

    PriceDisplayComponent.h
    Created: 17 Jun 2021 9:16:08am
    Author:  Eloi GUIHARD-GOUJON

    Contient les classes pour l'affichage de tous les prix.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GridTool.h"
#include "PriceComponent.h"

//==============================================================================
/*
*/

/// <summary>
/// Component affichant plusieurs PriceComponents
/// </summary>
class PriceDisplayComponent  : public juce::Component
{
    static constexpr size_t MAX_PRICES = 10;
public:
    PriceDisplayComponent();
    ~PriceDisplayComponent() override;

    // Initialisation
    void init();

    void paint (juce::Graphics&) override;
    void resized() override;

    // Change le nombre de prix
    void setNumPrices(unsigned int num_of_prices);

    // Change le nombre de chiffres
    void setNumDigits(unsigned int num_of_digits);

    // Ajoute un prix
    void addPrice();

    // Retourne le rectangle dans lequel doit être affiché les prix
    juce::Rectangle<int> getFittingRectangle(const juce::Rectangle<int>& r);

    // Met à jour les prix affichés
    void updatePrices(TextUpdateOrigin whoCalled, unsigned int priceIndex);

    // Change l'état d'erreur d'un module
    void setModuleState(int moduleNumber, const ErrModule& newState);

    // Retourne le n-ième PriceComponent
    PriceComponent* operator[](size_t index) { return &prices[index]; }

private:
    PriceComponent prices[MAX_PRICES];
    GridTool grid;
    size_t numPrices;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PriceDisplayComponent)
};