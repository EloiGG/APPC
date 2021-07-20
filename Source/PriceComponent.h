/*
  ==============================================================================

	PriceComponent.h
	Created: 17 Jun 2021 9:16:21am
	Author:  Eloi GUIHARD-GOUJON

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

class PriceEditorLabel : public SpecialLabel
{
public:
	PriceEditorLabel() {}
protected:
	virtual void editorShown(TextEditor* te) override
	{
		Core::get().showKeyboard((SpecialLabel*)this, te->getText().substring(0, Core::get().getNumDigits() + 1), Core::MAX_DIGITS);
	}
	virtual void editorAboutToBeHidden(TextEditor*) override
	{
		if (lastText != getText()) {
			const auto& str = getText();
			if (str.containsOnly(Core::get().getDigitEditorAcceptedCharacters()))
				lastText = str;
			setText(lastText, NotificationType::sendNotification);
			if (textManuallyUpdated)
				textManuallyUpdated();
		}
	}
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PriceEditorLabel)
};

class PriceEditor : public PriceEditorLabel
{
public:
	PriceEditor();
	void setPrice(const String& newPrice);
	void paint(juce::Graphics&) override;
	void resized() override;
	void setNumberOfDigits(int new_number_of_digits);

protected:
	virtual void editorAboutToBeHidden(TextEditor*) override;
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
