/*
  ==============================================================================

	InputComponent.h
	Created: 21 Jun 2021 2:24:45pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Core.h"
#include "SpecialLabel.h"
#include "Log.h"

//==============================================================================
/*
*/

/// <summary>
/// Classe d�riv�e de KeyboardLabel pour l'�dition de texte via clavier ou clavier visuel
/// </summary>
class InputComponentLabel : public KeyboardLabel
{
public:
	InputComponentLabel() : maxNumChar(1) {}
	InputComponentLabel(const std::function<void()>& f) : maxNumChar(1)
	{
		textManuallyUpdated = f;
		onTextKeyboardUpdate = f;
	}
	unsigned int maxNumChar;
protected:
	virtual void editorShown(TextEditor* te) override
	{
		Core::get().showKeyboard(this, [](const String& output)
			{
				return output.containsOnly("0987654321");
			},
			te->getText(), maxNumChar);
	}
private:
};

/// <summary>
/// Component qui permet l'�dition d'un param�tre via un champs de texte �ditable par clavier,
/// clavier visuel ou via des boutons + et -
/// </summary>
class InputComponent : public juce::Component
{
public:
	InputComponent(const String& parameterName, const String& defaultValue = "");
	InputComponent(const String& parameterName, const int& defaultValue);
	~InputComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	// Change le nom du param�tre
	void setTitle(const String& newTitle) { title.setText(newTitle, NotificationType::sendNotification); }

	// Change la valeur du param�tre
	void setInput(const String& newInput) { input.setText(newInput, NotificationType::sendNotification); }

	// Change le nombre maximal de caract�res accept�s
	void setMaxInputLengh(unsigned int maxLengh) { input.maxNumChar = maxLengh; }

	// Action d�clench�e lorsqu'on clique sur le bouton +
	std::function<void()>& onIncrement;

	// Action d�clench�e lorsqu'on clique sur le bouton -
	std::function<void()>& onDecrement;

	// Action d�clench�e quand le texte est chang�
	std::function<void(const String&)> onUpdate;

	float min, max;
	int increment, decrement;

private:
	String lastText;
	Label title;
	InputComponentLabel input;
	TextButton p, m;
	int titleWidth;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputComponent)
};

/// <summary>
/// Component de type case � cocher
/// </summary>
class CheckBox : public ToggleButton
{
public:
	CheckBox(const String& text);
private:
	int titleWidth;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CheckBox)
};