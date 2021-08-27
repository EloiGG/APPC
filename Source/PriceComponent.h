/*
  ==============================================================================

	PriceComponent.h
	Created: 17 Jun 2021 9:16:21am
	Author:  Eloi GUIHARD-GOUJON

	Contient les classes nécessaires à l'affichage d'un seul prix

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

class PriceEditorLabel : public KeyboardLabel
{
public:
	PriceEditorLabel() {}

protected:
	virtual void editorShown(TextEditor* te) override
	{
		Core::get().showKeyboard(this, [](const String& output) {return Price::isValid(output); }, te->getText().substring(0, Core::get().getNumDigits() + 1), Core::MAX_DIGITS);
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

/// <summary>
/// Component derrière les chiffres qui permet d'éditer tout un prix
/// </summary>
class PriceEditor : public PriceEditorLabel
{
public:
	PriceEditor();

	// Change le prix
	void setPrice(const String& newPrice);

	// Change le nombre de chiffres
	void setNumberOfDigits(int new_number_of_digits);

	void paint(juce::Graphics&) override;
	void resized() override;

protected:
	virtual void editorAboutToBeHidden(TextEditor*) override;
	virtual void mouseDown(const MouseEvent&) override;
	virtual void textKeyboardUpdated() override;

private:
	bool isTextEditing;
	int numDigits;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PriceEditor)
};

/// <summary>
/// Component d'affichage d'un prix
/// </summary>
class PriceComponent : public juce::Component, private Timer
{
public:
	PriceComponent(unsigned int priceID = 0);
	~PriceComponent() override;

	// Initialisation
	void init();

	void paint(juce::Graphics&) override;
	void resized() override;

	// Change le prix
	void setPrice(const Price& newPrice);

	// Change le nombre de chiffres
	void setNumberOfDigits(int number_of_digits);

	// Change l'ID du prix
	void setID(unsigned int newID);

	// Met à jour les prix
	void updatePrices(TextUpdateOrigin whoCalled, unsigned int priceIndex);

	// Change l'état d'erreur du module spécifié
	void setModuleState(int digitNumber, const ErrModule& newState);

	// Retourne l'état d'erreur du module spécifié
	ErrModule getModuleState(int digitNumber);

private:
	// Met à jour l'affichage des chiffres
	void updateDigits();

	// Met à jour l'éditeur de prix derrière les chiffres
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
