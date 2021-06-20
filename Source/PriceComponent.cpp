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
PriceComponent::PriceComponent(unsigned int priceID, int number_of_digits) :  grid(number_of_digits, 1),
onPriceUpdate(false), ID(priceID), onPriceEditorUpdate(false), currentPrice(Price("0")), updatingPriceEditor(true), updatingDigits(false)
{
	setNumberOfDigits(number_of_digits);
	init();
}

void PriceComponent::init()
{
	priceEditor.onTextChange = [this]()
	{
		if (!updatingDigits) {
			updatingPriceEditor = true;
			Price p(priceEditor.getText());
			if (!p.isEmpty()) {
				currentPrice = p;
				updateDigits();
			}
			priceEditor.setText(currentPrice.toString(numDigits), NotificationType::sendNotification);
		}
	};

	for (int i = 0; i < numDigits; i++) digits[i].onTextChange = [this, i]()
	{
		if (!updatingPriceEditor) {
			if (Price::isValid(digits[i].getText()) && digits[i].getText().length() == 1) {
				updatingDigits = true;
				currentPrice.changeOneDigit(i, digits[i].getText());
				updatePriceEditor(currentPrice);
				updatingDigits = false;
			}
			else
				digits[i].setText(String(currentPrice.priceString[i] - 0x30), NotificationType::sendNotification);
		}
	};

	addAndMakeVisible(priceEditor);
	for (int i = 0; i < numDigits; i++)
		addAndMakeVisible(digits[i]);

	setPrice(currentPrice);
}

PriceComponent::~PriceComponent()
{
}

void PriceComponent::setTabOrder(int order)
{
	priceEditor.setExplicitFocusOrder(order);
}


void PriceComponent::paint(juce::Graphics& g)
{
}

void PriceComponent::setPrice(const Price& newPrice)
{
	currentPrice = newPrice;
	priceEditor.setPrice(newPrice.toString(numDigits));
}

void PriceComponent::setNumberOfDigits(int number_of_digits)
{
	if (number_of_digits <= Core::MAX_DIGITS && number_of_digits != numDigits) {
		grid.resize(number_of_digits, grid.getNumRows());
		numDigits = number_of_digits;
		priceEditor.setNumberOfDigits(number_of_digits);
		Core::get().setNumDigits(numDigits);
		init();
	}
}

void PriceComponent::setID(unsigned int newID)
{
	ID = newID;
	for (int i = 0; i < numDigits; i++) {
		digits[i].setExplicitFocusOrder((i + 1) + 4 * ID + 4);
	}
	DBG(getExplicitFocusOrder());
}

void PriceComponent::hideDigits(bool should)
{
	for (int i = 0; i < numDigits; i++)
		digits[i].setVisible(!should);
}

int PriceComponent::getNumDigits()
{
	return numDigits;
}

void PriceComponent::updateDigits()
{
	startTimerHz(4);
	timerCallback();
}

void PriceComponent::updatePriceEditor(const Price& newPrice)
{
	priceEditor.setPrice(newPrice.toString(numDigits));
}

void PriceComponent::timerCallback()
{
	if (updatingPriceEditor)
	{
		int i = 0;
		for (; i < numDigits; i++) {
			if (digits[i].getDigit() != currentPrice[i]) {
				digits[i].setDigit(currentPrice[i]);
				return;
			}
		}
		onPriceUpdate = false;
		//fin de mise à jour
		updatingPriceEditor = false;
		updatingDigits = false;
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

PriceEditor::PriceEditor() :isTextEditing(false), numDigits(4)
{
	setOpaque(true);
	setEditable(true);
	setBroughtToFrontOnMouseClick(true);
	setJustificationType(Justification::centred);
	setRepaintsOnMouseActivity(true);
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
	if (!isTextEditing) {
		auto size = getHeight() / 6;
		float rounding = 3.0f;
		auto rect = Rectangle(0, 0, size, size).withCentre({ getWidth() / numDigits, getHeight() * 3 / 4 }).toFloat();
		g.setColour(lfColours::digitColour);
		g.fillRoundedRectangle(rect, rounding);
	}
}

void PriceEditor::resized()
{
	Label::resized();
	auto f = getFont();
	f.setHeight(getHeight());
	setFont(f);
}

void PriceEditor::setNumberOfDigits(int new_number_of_digits)
{
	numDigits = new_number_of_digits;
}


void PriceEditor::editorAboutToBeHidden(TextEditor*)
{
	setAlwaysOnTop(false);
	isTextEditing = false;
	toBack();
}

void PriceEditor::editorShown(TextEditor*)
{
	setAlwaysOnTop(true);
	isTextEditing = true;
	DBG(getExplicitFocusOrder());
}

void PriceEditor::mouseDown(const MouseEvent&)
{
	showEditor();
}
