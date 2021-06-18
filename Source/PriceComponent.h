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
	virtual void editorAboutToBeHidden(TextEditor*) override;
	virtual void editorShown(TextEditor*);


private:
	String price;
	bool isTextEditing;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PriceEditor)
};

class PriceComponent : public juce::Component, private Timer
{
	static constexpr size_t MAX_DIGITS = 6;
public:
	PriceComponent(unsigned int priceID = 0, int number_of_digits = 4);
	~PriceComponent() override;

	void setTabOrder(int order);

	void paint(juce::Graphics&) override;
	void resized() override;

	void setPrice(const Price& newPrice);
	void setNumberOfDigits(int number_of_digits = 4);
	void setID(unsigned int newID);
	virtual void mouseDoubleClick(const MouseEvent& event) override;

	void hideDigits(bool shouldHideDigits);

private:
	void updateDigits();
	void updatePriceEditor(const Price& newPrice);

	int numDigits, ID;
	DigitEditor digits[MAX_DIGITS];
	GridTool grid;
	PriceEditor priceEditor;
	Price currentPrice;
	bool onPriceUpdate, onPriceEditorUpdate;
	bool updatingDigits, updatingPriceEditor;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PriceComponent)

		// Hérité via Timer
		virtual void timerCallback() override;
};
