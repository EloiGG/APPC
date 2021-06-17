/*
  ==============================================================================

	DigitComponent.cpp
	Created: 17 Jun 2021 11:04:03am
	Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DigitComponent.h"

//==============================================================================
DigitComponent::DigitComponent()
{
	addAndMakeVisible(digit);
	digit.setText(std::to_string((unsigned int)Random::getSystemRandom().nextInt() % 10), NotificationType::sendNotification);
	digit.setJustificationType(Justification::centred);
	digit.setFont(font);
	digit.setColour(Label::ColourIds::textColourId, Colour(244, 255, 143));
	font = Font("Seven Segment", "Regular", getHeight());
	setRepaintsOnMouseActivity(true);
}

DigitComponent::~DigitComponent()
{
}

void DigitComponent::paint(juce::Graphics& g)
{	
	g.setColour(Colours::black);
	g.fillAll();

	g.setColour(Colours::white);
	g.drawRect(getLocalBounds());
}

void DigitComponent::resized()
{
	digit.setBounds(getLocalBounds());
	font.setHeight(getHeight());
	digit.setFont(font);
}

DigitEditor::DigitEditor()
{
	setRepaintsOnMouseActivity(true);
	setEditable(true);
}

void DigitEditor::paint(juce::Graphics& g)
{
	auto c = isMouseOver() ? lfColours::digitBackground.brighter() : lfColours::digitBackground;
	g.fillAll(c);
	Label::paint(g);
	g.setColour(Colours::white);
	g.drawRect(getLocalBounds());
}

void DigitEditor::setDigit(String& newDigit)
{
	setText(newDigit, NotificationType::sendNotification);
}

void DigitEditor::mouseDown(const MouseEvent&)
{

}
