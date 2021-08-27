/*
  ==============================================================================

	DigitComponent.h
	Created: 17 Jun 2021 11:04:03am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"
#include "Core.h"
#include "SpecialLabel.h"
#include "SerialThread.h"

//==============================================================================
/*
*/

/// <summary>
/// Classe héritée de KeyboardLabel pour l'édition d'un chiffre d'un prix
/// </summary>
class DigitEditorLabel : public KeyboardLabel
{
public:
	DigitEditorLabel() {
		setEditable(true, true, true);
		setWantsKeyboardFocus(false);
	}

protected:
	virtual void editorShown(TextEditor* te) override
	{
		Core::get().showKeyboard(this, [](const String& output) {return Price::isValid(output); }, te->getText(), 1);
	}
	virtual void editorAboutToBeHidden(TextEditor*) override
	{
		if (lastText.length() && lastText != getText()) {
			const auto& digit = getText().substring(0, 1);
			if (digit.containsOnly(Core::get().getDigitEditorAcceptedCharacters()))
				lastText = digit;
			setText(lastText, NotificationType::sendNotification);
			textManuallyUpdated();
		}
	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DigitEditorLabel)
};

/// <summary>
/// Component pour l'affichage et l'édition de chiffres d'un prix
/// Permet aussi d'afficher l'état d'erreur du module 7 segments qu'il représente
/// </summary>
class DigitEditor : public DigitEditorLabel, private Timer
{
public:
	DigitEditor();

	void paint(juce::Graphics&) override;
	void resized() override;

	// Change le chiffre affiché
	void setDigit(const String& newDigit);

	// Retourne le chiffre affiché par ce module
	String getDigit();

	// Change l'état d'erreur du module
	void setState(const ErrModule& newState);

	// Retourne l'état d'erreur du module
	ErrModule getState() { return state; }

	// Choisir si le module doit afficher son état d'erreur
	void setShowState(bool shouldShowState);

	// Joue l'animation de mise à jour du chiffre
	void updateAnimation();

	//
	virtual void textKeyboardUpdated() override;

private:
	bool hasState;
	virtual void timerCallback() override;
	unsigned int frameCounter;
	ErrModule state;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DigitEditor)
};

