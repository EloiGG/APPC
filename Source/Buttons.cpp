/*
  ==============================================================================

	Buttons.cpp
	Created: 21 Jun 2021 12:37:46pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Buttons.h"

//==============================================================================
Buttons::Buttons() : send("Envoyer"), stop("Stop"), prixP("Prix + 1"), prixM("Prix - 1"), digitP("Chiffre + 1"), digitM("Chiffre - 1"),
grid(4, 1)
{
	addAndMakeVisible(grid);


	send.setLookAndFeel(Core::get().getLookAndFeel().get());
	stop.setLookAndFeel(Core::get().getLookAndFeel().get());

	send.setColour(TextButton::ColourIds::buttonColourId, Colours::green);
	stop.setColour(TextButton::ColourIds::buttonColourId, Colours::red);

	prixP.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonColor);
	prixM.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonColor);
	digitP.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonColor);
	digitM.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonColor);

	addAndMakeVisible(send);
	addAndMakeVisible(stop);
	/*addAndMakeVisible(prixP);
	addAndMakeVisible(prixM);
	addAndMakeVisible(digitP);
	addAndMakeVisible(digitM);*/
}

Buttons::~Buttons()
{
}

void Buttons::paint(juce::Graphics& g)
{
}

void Buttons::resized()
{
	float separation = 0.2;
	grid.setBounds(getLocalBounds());
	send.setBounds(grid.getRectangle(1, 0, 4, 1));
	stop.setBounds(grid.getRectangle(0, 0, 1, 1));
	/*prixP.setBounds(grid.getRectangle(0, 0, 1 - separation / 2, 1));
	prixM.setBounds(grid.getRectangle(1 - separation / 2, 0, 2 - separation, 1));
	digitP.setBounds(grid.getRectangle(2, 0, 3 + separation, 1));
	digitM.setBounds(grid.getRectangle(3 + separation / 2, 0, 4, 1));*/
}
