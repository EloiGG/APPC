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

//==============================================================================
/*
*/

class PriceEditor : public Label
{
public:
	PriceEditor();
	void setPrice(const String& newPrice);
	void paint(juce::Graphics&) override;

protected:
	virtual void mouseDown(const MouseEvent& event) override;
	virtual void editorAboutToBeHidden(TextEditor*) override;

private:
	String price;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PriceEditor)
};

class PriceComponent : public juce::Component
{
	static constexpr size_t MAX_DIGITS = 6;
public:
	PriceComponent(int number_of_digits = 4);
	~PriceComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	void setNumberOfDigits(int number_of_digits = 4);

	virtual void mouseDoubleClick(const MouseEvent& event) override;

private:
	int numDigits;
	DigitComponent digits[MAX_DIGITS];
	GridTool grid;
	PriceEditor priceEditor;
	Font priceEditorFont;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PriceComponent)
};
