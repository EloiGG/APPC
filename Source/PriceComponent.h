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


protected:
	virtual void mouseDown(const MouseEvent& event) override;
	virtual void editorAboutToBeHidden(TextEditor*) override;

private:
	String price;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PriceEditor)
};

class PriceComponent : public juce::Component, private Timer
{
	static constexpr size_t MAX_DIGITS = 6;
public:
	PriceComponent(int number_of_digits = 4);
	~PriceComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	void setPrice(Price& newPrice);
	void setNumberOfDigits(int number_of_digits = 4);

	virtual void mouseDoubleClick(const MouseEvent& event) override;

private:
	void updateDigits(Price& newPrice);
	//void updatePriceEditor(Price& newPrice);

	int numDigits;
	DigitEditor digits[MAX_DIGITS];
	GridTool grid;
	PriceEditor priceEditor;
	Price currentPrice;
	bool onPriceUpdate;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PriceComponent)

		// Hérité via Timer
		virtual void timerCallback() override;
};