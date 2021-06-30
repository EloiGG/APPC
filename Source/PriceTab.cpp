/*
  ==============================================================================

    PriceTab.cpp
    Created: 21 Jun 2021 12:38:08pm
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PriceTab.h"

//==============================================================================
PriceTab::PriceTab() : grid(4, 12),
id("ID"), price("Prix"), gasStation("Station"), fuel("Carburant"), created(L"Créé"), updated(L"Mis à jour"),
position("Position"), emptied(L"Vidé"), modifyAt("\"ModyfyAt\""), modifyDone("Modification faite"), 
status("Statut"), oldPrice("Dernier prix")
{
    grid.setBounds(0, 0, getWidth(), 300);

    addAndMakeVisible(id);
    addAndMakeVisible(price);
    addAndMakeVisible(gasStation);
    addAndMakeVisible(fuel);
    addAndMakeVisible(created);
    addAndMakeVisible(updated);
    addAndMakeVisible(position);
    addAndMakeVisible(emptied);
    addAndMakeVisible(modifyAt);
    addAndMakeVisible(modifyDone);
    addAndMakeVisible(status);
    addAndMakeVisible(oldPrice);
}

PriceTab::~PriceTab()
{
}

void PriceTab::init(const PriceJSON& json)
{
    id.setInput(String(json.getId()));
}

void PriceTab::paint (juce::Graphics& g)
{
    g.fillAll(lfColours::tabBackground);
}

void PriceTab::resized()
{
    grid.setBounds(grid.getLocalBounds().withWidth(getWidth()));

    id.setBounds(grid.getLine(0));
    price.setBounds(grid.getLine(1));
    gasStation.setBounds(grid.getLine(2));
    fuel.setBounds(grid.getLine(3));
    created.setBounds(grid.getLine(4));
    updated.setBounds(grid.getLine(5));
    position.setBounds(grid.getLine(6));
    emptied.setBounds(grid.getLine(7));
    modifyAt.setBounds(grid.getLine(8));
    modifyDone.setBounds(grid.getLine(9));
    status.setBounds(grid.getLine(10));
    oldPrice.setBounds(grid.getLine(11));
}
