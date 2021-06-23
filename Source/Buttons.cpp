/*
  ==============================================================================

	Buttons.cpp
	Created: 21 Jun 2021 12:37:46pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "Buttons.h"

//==============================================================================
Buttons::Buttons() : send("Envoyer"), stop("Stop"), grid(4, 1)
{
	addAndMakeVisible(grid);

	send.setLookAndFeel(Core::get().getLookAndFeel().get());
	stop.setLookAndFeel(Core::get().getLookAndFeel().get());

	send.setColour(TextButton::ColourIds::buttonColourId, Colours::green);
	stop.setColour(TextButton::ColourIds::buttonColourId, Colours::red);

	addAndMakeVisible(send);
	addAndMakeVisible(stop);

	send.onClick = [this]() {Sequence s;
	s.createSequence(Core::get());
	sendThread.startThread(); };
	stop.onClick = [this]() {sendThread.stopThread(2500); };
}

Buttons::~Buttons()
{
	sendThread.stopThread(1500);
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
}
