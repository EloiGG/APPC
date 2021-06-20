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

class PriceEditor : public Label
{
public:
	PriceEditor();
	void setPrice(const String& newPrice);
	void paint(juce::Graphics&) override;
	void resized() override;
	void setNumberOfDigits(int new_number_of_digits);

protected:
	virtual void editorAboutToBeHidden(TextEditor*) override;
	virtual void editorShown(TextEditor*);
	virtual void mouseDown(const MouseEvent&) override;

private:
	String price;
	bool isTextEditing;
	int numDigits;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PriceEditor)
};

class PriceComponent : public juce::Component, private Timer
{
public:
	PriceComponent(unsigned int priceID = 0, int number_of_digits = 4);
	~PriceComponent() override;
	void init();

	void setTabOrder(int order);

	void paint(juce::Graphics&) override;
	void resized() override;

	void setPrice(const Price& newPrice);
	void setNumberOfDigits(int number_of_digits = 4);
	void setID(unsigned int newID);

	void hideDigits(bool shouldHideDigits);
	int getNumDigits();

private:
	void updateDigits();
	void updatePriceEditor(const Price& newPrice);

	int numDigits, ID;
	DigitEditor digits[Core::MAX_DIGITS];
	GridTool grid;
	PriceEditor priceEditor;
	Price currentPrice;
	bool onPriceUpdate, onPriceEditorUpdate;
	bool updatingDigits, updatingPriceEditor;
	virtual void timerCallback() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PriceComponent)
};
