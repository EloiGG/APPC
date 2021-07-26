/*
  ==============================================================================

	Keyboard.h
	Created: 13 Jul 2021 2:08:34pm
	Author:  Eloi GUIHARD-GOUJON

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
class Key : public TextButton
{
	bool keyPressed(const KeyPress&) override { return false; }
};

class Keyboard : public juce::Component, public KeyListener/*, public ApplicationCommandTarget*/
{
public:
	static constexpr size_t NUM_ROWS = 4;
	static constexpr size_t NUM_COLUMNS = 4;
	static constexpr size_t NUM_KEYS = NUM_ROWS * NUM_COLUMNS;

public:
	Keyboard();
	~Keyboard() override;
	void resetAndShow(KeyboardLabel* caller, const String& startingText = "", unsigned int maxNumberOfCharacters = Core::MAX_DIGITS);
	void paint(juce::Graphics&) override;
	void resized() override;
	String getOutput() { return output; }
	void setText(const String& newText) { label.setText(newText, NotificationType::sendNotification); }

	//virtual ApplicationCommandTarget* getNextCommandTarget() override;
	//virtual void getAllCommands(Array<CommandID>& commands) override;
	//virtual void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
	//virtual bool perform(const InvocationInfo& info) override;

private:
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
