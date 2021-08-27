/*
  ==============================================================================

	SpecialLabel.h
	Created: 23 Jun 2021 9:27:16am
	Author:  Eloi GUIHARD-GOUJON

	Contient des versions personnalisées de la classe Label qui sont utilisées
	par plusieurs components dans le programme.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <functional>

/// <summary>
/// Label qui appelle la fonction lambda textManuallyUpdated lorsque le texte a été changé
/// </summary>
class SpecialLabel : public Label
{
public:
	SpecialLabel(const std::function<void()>& onTextUpdate) : textManuallyUpdated(onTextUpdate), lastText("") { }
	SpecialLabel() : lastText("") {}
	std::function<void()> textManuallyUpdated;

protected:
	virtual void editorAboutToBeHidden(TextEditor*) override
	{
		if (lastText != getText()) {
			if (textManuallyUpdated)
				textManuallyUpdated();
			lastText = getText();
		}
	}
	String lastText;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpecialLabel)
};

/// <summary>
/// Label permettant l'affichage du clavier visuel lorsqu'on clique dessus
/// </summary>
class KeyboardLabel : public SpecialLabel
{
public:
	KeyboardLabel() = default;
	void callForUpdate() { textKeyboardUpdated(); if (onTextKeyboardUpdate) onTextKeyboardUpdate(); }

	// Spécifier cette fonction pour spécifer une action lorsque le texte est mis à jour 
	std::function<void()> onTextKeyboardUpdate;
protected:
	// Surcharger cette fonction pour spécifer une action lorsque le texte est mis à jour
	virtual void textKeyboardUpdated() {}

	virtual void editorAboutToBeHidden(TextEditor*) override {}
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyboardLabel)
};