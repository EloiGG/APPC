/*
  ==============================================================================

	PriceComponent.cpp
	Created: 17 Jun 2021 9:16:21am
	Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PriceComponent.h"
#include "LookAndFeel.h"

//==============================================================================
PriceComponent::PriceComponent(int number_of_digits) : numDigits(number_of_digits), grid(number_of_digits, 1),
onPriceUpdate(false)
{
	priceEditor.onTextChange = [this]()
	{
		updateDigits(Price(priceEditor.getText()));
	};

	addAndMakeVisible(priceEditor);
	for (int i = 0; i < numDigits; i++)
		addAndMakeVisible(digits[i]);
}

PriceComponent::~PriceComponent()
{
}


void PriceComponent::paint(juce::Graphics& g)
{
}


void PriceComponent::setPrice(Price& newPrice)
{
	priceEditor.setPrice(newPrice.toString(4));
}

void PriceComponent::setNumberOfDigits(int number_of_digits)
{
	grid.resize(number_of_digits, grid.getNumRows());
	numDigits = number_of_digits;
}

void PriceComponent::mouseDoubleClick(const MouseEvent& event)
{
	priceEditor.setVisible(false);
	if (priceEditor.isBeingEdited())
		DBG("edit");
}

void PriceComponent::updateDigits(Price& newPrice)
{
	currentPrice = newPrice;
	onPriceUpdate = true;
	startTimerHz(2);
	timerCallback();
}

void PriceComponent::timerCallback()
{
	if (onPriceUpdate)
	{
		DBG("timer callback");
		int i = 0;
		for (; i < numDigits; i++) {
			if (digits[i].getDigit() != currentPrice[i]) {
				digits[i].setDigit(currentPrice[i]);
				return;
			}
		}
		onPriceUpdate = false;
		stopTimer();
	}
}


void PriceComponent::resized()
{
	grid.setBounds(getLocalBounds());
	float whRatio = 4.0f / 5.0f;
	for (int i = 0; i < numDigits; i++) {
		auto rect = grid.getRectangle(i, 0, i + 1, 1);
		auto rect2 = rect.withWidth(rect.getHeight() * whRatio).withCentre(rect.getCentre());
		digits[i].setBounds(rect2);
	}
	priceEditor.setBounds(getLocalBounds());
}

PriceEditor::PriceEditor()
{
	setOpaque(true);
	setEditable(true);
	setBroughtToFrontOnMouseClick(true);
	setJustificationType(Justification::centred);
	setRepaintsOnMouseActivity(true);
	setText("333", NotificationType::sendNotification);
	setColour(Label::ColourIds::textColourId, lfColours::digitColour.withAlpha(0.0f));
	setFont(Font("Seven Segment", "Regular", getHeight()));
}

void PriceEditor::setPrice(const String& newPrice)
{
	setText(newPrice, NotificationType::sendNotification);
}

void PriceEditor::paint(juce::Graphics& g)
{
	auto c = isMouseOverOrDragging(true) ? lfColours::priceBackground.brighter() : lfColours::priceBackground;
	g.fillAll(c);
	Label::paint(g);
}

void PriceEditor::resized()
{
	Label::resized();
	auto f = getFont();
	f.setHeight(getHeight());
	setFont(f);
}

void PriceEditor::mouseDown(const MouseEvent& m)
{
	showEditor();
}

void PriceEditor::editorAboutToBeHidden(TextEditor*)
{
	setAlwaysOnTop(false);
	toBack();
}
