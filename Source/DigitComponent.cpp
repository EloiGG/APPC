/*
  ==============================================================================

	DigitComponent.cpp
	Created: 17 Jun 2021 11:04:03am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DigitComponent.h"

//==============================================================================

DigitEditor::DigitEditor() : frameCounter(0)
{
	setEditable(true);
	setRepaintsOnMouseActivity(true);
	setOpaque(true);

	setJustificationType(Justification::centred);
	setFont(Font("Seven Segment", "Regular", getHeight()));
	setColour(Label::ColourIds::textColourId, lfColours::digitColour);
}

void DigitEditor::paint(juce::Graphics& g)
{
	auto c = isMouseOver() ? lfColours::digitBackground.brighter() : lfColours::digitBackground;
	g.fillAll(c);
	g.setColour(lfColours::digitColour);
	Label::paint(g);
	g.setColour(Colours::white);
	g.drawRect(getLocalBounds());

	float updateRectangleMargin = getWidth() / 10, cornerSize = getHeight() / 40.0f;
	if (frameCounter>0) {
		g.setColour(lfColours::onDigitUpdate.withAlpha(jmap<float>(frameCounter, 0, 60, 1, 0)));
		g.drawRoundedRectangle(getLocalBounds().reduced(updateRectangleMargin).toFloat(), cornerSize, 1.0f);
	}
}

void DigitEditor::setDigit(const String& newDigit)
{
	setText(newDigit.substring(0, 1), NotificationType::sendNotification);
	frameCounter = 0;
	startTimerHz(60);
	timerCallback();
}

String DigitEditor::getDigit()
{
	return getText().substring(0, 1);
}

void DigitEditor::resized()
{
	Label::resized();
	auto f = getFont();
	f.setHeight(getHeight());
	setFont(f);
}

void DigitEditor::timerCallback()
{
	repaint();
	if (++frameCounter > 60) {
		frameCounter = 0;
		stopTimer();
	}
}
