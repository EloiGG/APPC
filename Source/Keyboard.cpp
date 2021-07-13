/*
  ==============================================================================

	Keyboard.cpp
	Created: 13 Jul 2021 2:08:34pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Keyboard.h"

//==============================================================================
Keyboard::Keyboard() : grid(NUM_COLUMNS, NUM_ROWS + 1), currentMaxNumChar(Core::MAX_DIGITS)
{
	auto p = &lf;
	auto c = lfColours::buttonBackground, annuler = Colour(166, 30, 30), confirmer = Colour(66, 171, 56), retour = Colour(204, 164, 71);
	label.setEditable(false, false);

	for (int y = 0; y < NUM_ROWS; y++)
		for (int x = 0; x < NUM_COLUMNS; x++) {
			int i = y * NUM_ROWS + x;
			addAndMakeVisible(key[i]);
			key[i].setLookAndFeel(p);
			if (x < 3 && y < 3) {
				key[i].setColour(TextButton::ColourIds::buttonColourId, c);
				key[i].setButtonText(String(3 * y + x + 1));
			}
			else
				key[i].setColour(TextButton::ColourIds::buttonColourId, c.darker(0.2f));
			key[i].onClick = [this, i]()
			{
				String add = (label.getText().length() <= currentMaxNumChar) ? key[i].getButtonText() : String();
				label.setText(label.getText() + add, NotificationType::sendNotification);
			};
		}

	key[3].setButtonText(" ");
	key[7].setButtonText("-");
	key[12].setButtonText(".");

	key[11].setButtonText("X");
	key[11].onClick = [this]() {setVisible(false); DBG("\"" << output << "\""); };
	key[11].setColour(TextButton::ColourIds::buttonColourId, annuler);

	key[13].setButtonText("0");
	key[13].setColour(TextButton::ColourIds::buttonColourId, c);

	key[14].setButtonText("<<");
	key[14].onClick = [this]() {label.setText(label.getText().substring(0, label.getText().length() - 1), NotificationType::sendNotification); };
	key[14].setColour(TextButton::ColourIds::buttonColourId, retour);

	key[15].setButtonText("OK");
	key[15].onClick = [this]() {output = label.getText(); if (output.length() == 0) output = " "; setVisible(false); DBG("\"" << output << "\""); };
	key[15].setColour(TextButton::ColourIds::buttonColourId, confirmer);

	label.setFont(Font("Seven Segment", 30, 0));
	addAndMakeVisible(label);
}

Keyboard::~Keyboard()
{
}

void Keyboard::resetAndShow(unsigned int maxNumberOfCharacters)
{
	label.setText("", NotificationType::sendNotification);
	output = "";
	setVisible(false);
	currentMaxNumChar = maxNumberOfCharacters;
}

void Keyboard::paint(juce::Graphics& g)
{
	g.fillAll(Colours::black);
}

void Keyboard::resized()
{
	grid.setBounds(getLocalBounds());
	for (int y = 0; y < NUM_ROWS; y++)
		for (int x = 0; x < NUM_COLUMNS; x++)
			key[y * NUM_ROWS + x].setBounds(grid.getRectangle(x, y, x + 1, y + 1));
	label.setBounds(grid.getLine(grid.getNumRows() - 1));
}
