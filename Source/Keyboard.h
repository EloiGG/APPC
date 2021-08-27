/*
  ==============================================================================

	Keyboard.h
	Created: 13 Jul 2021 2:08:34pm
	Author:  Eloi GUIHARD-GOUJON

	Contient les classes nécessaires à l'affichage du clavier visuel

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Core.h"
#include "GridTool.h"
#include "LookAndFeel.h"
#include "SpecialLabel.h"
#include "DigitComponent.h"
#include "PriceComponent.h"
#include "Disabled.h"
//==============================================================================
/*
*/

// Touche du clavier visuel
class Key : public TextButton
{
	bool keyPressed(const KeyPress&) override { return false; }
};

/// <summary>
/// Component clavier visuel permettant l'édition des champs de texte sans avoir à utiliser de clavier d'ordinateur
/// </summary>
class Keyboard : public juce::Component, public KeyListener/*, public ApplicationCommandTarget*/
{
public:
	static constexpr size_t NUM_ROWS = 4;
	static constexpr size_t NUM_COLUMNS = 4;
	static constexpr size_t NUM_KEYS = NUM_ROWS * NUM_COLUMNS;

public:
	Keyboard();
	~Keyboard() override;

	// Montre le clavier visuel après l'avoir réinitialisé
	void resetAndShow(KeyboardLabel* caller, const std::function<bool(const String&)>& validationFunction, 
		const String& startingText = "", unsigned int maxNumberOfCharacters = Core::MAX_DIGITS);

	void paint(juce::Graphics&) override;
	void resized() override;

	// Retourne le texte entré dans le clavier visuel
	String getOutput() { return output; }

	// Change le texte dans le clavier visuel
	void setText(const String& newText) { label.setText(newText, NotificationType::sendNotification); }

	// Permet de définir une fonction qui détermine si le texte entré est valide ou non
	void setValidationFunction(const std::function<bool(const String&)>& f) { textIsValid = f; }

private:
	 std::function<bool(const String&)> textIsValid;
	unsigned int currentMaxNumChar;
	String output;
	KeyboardLookAndFeel lf;
	Key key[NUM_KEYS];
	GridTool grid;
	PriceEditorLabel label;
	KeyboardLabel* caller;
	virtual bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
	Disabled disable;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Keyboard)

};
