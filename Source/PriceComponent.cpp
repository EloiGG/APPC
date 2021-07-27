/*
  ==============================================================================

	PriceComponent.cpp
	Created: 17 Jun 2021 9:16:21am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PriceComponent.h"
#include "LookAndFeel.h"

//==============================================================================
PriceComponent::PriceComponent(unsigned int priceID) : grid(Core::get().getNumDigits(), 1), ID(priceID),
currentPrice(Core::get().getPrice(ID))
{
	setNumberOfDigits(Core::get().getNumDigits());
	init();
}

void PriceComponent::init()
{
	priceEditor.onTextKeyboardUpdate = [this]()
	{
		Price p(priceEditor.getText());
		if (!p.isEmpty()) {
			currentPrice = p;
			Core::get().setPrice(ID, p);
			Core::get().updatePrices(TextUpdateOrigin::PriceEditor, ID);
		}
		priceEditor.setText(currentPrice.toString(Core::MAX_DIGITS), NotificationType::sendNotification);
	};

	for (int i = 0; i < numDigits; i++) digits[i].onTextKeyboardUpdate = [this, i]()
	{
		currentPrice.changeOneDigit(i, digits[i].getText());
		updatePriceEditor(currentPrice);
		digits[i].setState(ErrModule::notUpToDate());
		Core::get().setPrice(ID, currentPrice);
		Core::get().updatePrices(TextUpdateOrigin::PriceEditor, ID);
	};
	numDigits = Core::get().getNumDigits();
	addAndMakeVisible(priceEditor);
	for (int i = 0; i < numDigits; i++)
		addAndMakeVisible(digits[i]);

	for (int i = numDigits; i < Core::MAX_DIGITS; i++)
		digits[i].setVisible(false);

	setPrice(Core::get().getPrice(ID));

	updatePrices(TextUpdateOrigin::Omni, ID);

	if (getParentComponent()) // pricedisplay
		if (getParentComponent()->getParentComponent()) //middlepanel
			getParentComponent()->getParentComponent()->resized();

	resized();
}

PriceComponent::~PriceComponent()
{
}


void PriceComponent::paint(juce::Graphics& g)
{
}

void PriceComponent::setPrice(const Price& newPrice)
{
	currentPrice = newPrice;
	priceEditor.setPrice(newPrice.toString(Core::get().getNumDigits()));
}

void PriceComponent::setNumberOfDigits(int number_of_digits)
{
	if (number_of_digits <= Core::MAX_DIGITS && number_of_digits != numDigits) {
		grid.resize(number_of_digits, grid.getNumRows());
		numDigits = number_of_digits;
		//priceEditor.setNumberOfDigits(number_of_digits);
		//Core::get().setNumDigits(numDigits);
		init();
	}
}

void PriceComponent::setID(unsigned int newID)
{
	ID = newID;
	/*for (int i = 0; i < numDigits; i++) {
		digits[i].setExplicitFocusOrder((i + 1) + 4 * ID + 4);
	}*/
}

void PriceComponent::updatePrices(TextUpdateOrigin whoCalled, unsigned int priceIndex)
{
	if (priceIndex == ID)
	{
		currentPrice = Core::get().getPrice(ID);
		if (whoCalled != TextUpdateOrigin::PriceEditor)
			updatePriceEditor(currentPrice);
		if (whoCalled != TextUpdateOrigin::DigitEditor)
			updateDigits();
	}
}

void PriceComponent::setModuleState(int digitNumber, const ErrModule& newState)
{
	digits[digitNumber].setState(newState);
}

ErrModule PriceComponent::getModuleState(int digitNumber)
{
	return digits[digitNumber].getState();
}

void PriceComponent::updateDigits()
{
	for (int i = 0; i < numDigits; i++)
		if (!Core::get().isInit())
			digits[i].setState(ErrModule::white());
		else if (digits[i].getDigit() != currentPrice[i])
			digits[i].setState(ErrModule::notUpToDate());
	startTimerHz(4);
	timerCallback();
}

void PriceComponent::updatePriceEditor(const Price& newPrice)
{
	priceEditor.setPrice(newPrice.toString(Core::MAX_DIGITS));
}

void PriceComponent::timerCallback()
{
	currentPrice = Core::get().getPrice(ID);
	for (int i = 0; i < numDigits; i++) {
		if (digits[i].getDigit() != currentPrice[i]) {
			digits[i].setDigit(currentPrice[i]);
			return;
		}
	}
	stopTimer();
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


//=================================================================================================================
//=================================================================================================================
//													PRICE EDITOR
//=================================================================================================================
//=================================================================================================================


PriceEditor::PriceEditor() :isTextEditing(false), numDigits(Core::get().getNumDigits())
{
	setOpaque(true);
	setEditable(true);
	setBroughtToFrontOnMouseClick(true);
	setRepaintsOnMouseActivity(true);

	setJustificationType(Justification::centred);
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
		auto rect = juce::Rectangle(0, 0, size, size).withCentre({ getWidth() / (int)Core::get().getNumDigits(), getHeight() * 3 / 4 }).toFloat();
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


void PriceEditor::editorAboutToBeHidden(TextEditor* te)
{
	SpecialLabel::editorAboutToBeHidden(te);
	setAlwaysOnTop(false);
	isTextEditing = false;
	toBack();
}

//void PriceEditor::editorShown(TextEditor*)
//{
//	setAlwaysOnTop(true);
//	isTextEditing = true;
//}

void PriceEditor::mouseDown(const MouseEvent&)
{
	showEditor();
}

void PriceEditor::textKeyboardUpdated()
{
	setPrice(getText());
}
