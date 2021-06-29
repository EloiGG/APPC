/*
  ==============================================================================

	PriceComponent.h
	Created: 17 Jun 2021 9:16:21am
	Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DigitComponent.h"
#include "GridTool.h"
#include "Price.h"
#include "Core.h"

//==============================================================================
/*
*/

class PriceEditor : public SpecialLabel
{
public:
	PriceEditor();
	void setPrice(const String& newPrice);
	void paint(juce::Graphics&) override;
	void resized() override;
	void setNumberOfDigits(int new_number_of_digits);

protected:
	virtual void editorAboutToBeHidden(TextEditor*) override;
	virtual void editorShown(TextEditor*) override;
	virtual void mouseDown(const MouseEvent&) override;

private:
	bool isTextEditing;
	int numDigits;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PriceEditor)
};

class PriceComponent : public juce::Component, private Timer
{
public:
	PriceComponent(unsigned int priceID = 0);
	~PriceComponent() override;
	void init();

	void paint(juce::Graphics&) override;
	void resized() override;

	void setPrice(const Price& newPrice);
	void setNumberOfDigits(int number_of_digits);
	void setID(unsigned int newID);

	void updatePrices(TextUpdateOrigin whoCalled, unsigned int priceIndex);

	void setModuleState(int digitNumber, const ErrModule& newState);
private:
	void updateDigits();
	void updatePriceEditor(const Price& newPrice);
	virtual void timerCallback() override;

	int numDigits, ID;
	bool updatingDigits, updatingPriceEditor;
	Price currentPrice;

	DigitEditor digits[Core::MAX_DIGITS];
	PriceEditor priceEditor;
	GridTool grid;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PriceComponent)
};
