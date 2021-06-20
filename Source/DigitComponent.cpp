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
DigitComponent::DigitComponent()
{
	setJustificationType(Justification::centred);
	setFont(font);
	setColour(Label::ColourIds::textColourId, Colour(244, 255, 143));
	font = Font("Seven Segment", "Regular", getHeight());
	setRepaintsOnMouseActivity(true);
	setEditable(true);
	setWantsKeyboardFocus(false);
	Core::get().updatePrices(TextUpdateOrigin::PriceEditor);
}

DigitComponent::~DigitComponent()
{
}

//void DigitComponent::paint(juce::Graphics& g)
//{	
//	g.setColour(Colours::black);
//	g.fillAll();
//	Label::paint(g);
//	g.setColour(Colours::white);
//	g.drawRect(getLocalBounds());
//}

void DigitComponent::resized()
{
	font.setHeight(getHeight());
	setFont(font);
}

DigitEditor::DigitEditor() : frameCounter(0)
{
	setEditable(true);
	setRepaintsOnMouseActivity(true);
	setOpaque(true);
	setEditable(true);
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

void DigitEditor::editorShown(TextEditor*)
{
	DBG(getExplicitFocusOrder());
}

void DigitEditor::timerCallback()
{
	repaint();
	if (++frameCounter > 60) {
		frameCounter = 0;
		stopTimer();
	}
}

Digit::Digit()
{
	setText("2", NotificationType::sendNotification);
	setEditable(true);
	setFont(Font("Seven Segment", "Regular", 30));
}

void Digit::resized()
{
	Label::resized();
	//getFont().setHeight(getHeight());
}
