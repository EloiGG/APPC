/*
  ==============================================================================

	Keyboard.cpp
	Created: 13 Jul 2021 2:08:34pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Keyboard.h"

//==============================================================================
Keyboard::Keyboard() : grid(NUM_COLUMNS, NUM_ROWS + 1), currentMaxNumChar(Core::MAX_DIGITS)
{
	addAndMakeVisible(disable);
	disable.setDisabled(true);
	setOpaque(false);
	auto p = &lf;
	auto c = lfColours::buttonBackground, annuler = lfColours::keyboardCancel, confirmer = lfColours::keyboardConfirm, retour = lfColours::keyboardBack;
	label.setEditable(false);
	addKeyListener(this);
	for (int y = 0; y < NUM_ROWS; y++)
		for (int x = 0; x < NUM_COLUMNS; x++) {
			int i = y * NUM_ROWS + x;
			addAndMakeVisible(key[i]);
			key[i].clearShortcuts();
			key[i].setLookAndFeel(p);
			if (x < 3 && y < 3) {
				key[i].setColour(TextButton::ColourIds::buttonColourId, c);
				key[i].setButtonText(String(3 * y + x + 1));
			}
			else
				key[i].setColour(TextButton::ColourIds::buttonColourId, c.darker(0.2f));
			key[i].onClick = [this, i]()
			{
				if (currentMaxNumChar == 0)
					label.setText(key[i].getButtonText(), NotificationType::sendNotification);
				else {
					String add = (label.getText().length() <= currentMaxNumChar) ? key[i].getButtonText() : String();
					label.setText(label.getText() + add, NotificationType::sendNotification);
				}
			};
		}

	key[3].setButtonText(" ");
	key[7].setButtonText("-");
	key[12].setButtonText(".");

	key[11].setButtonText("X");
	key[11].onClick = [this]() {setVisible(false); DBG("\"" << output << "\""); };
	key[11].setColour(TextButton::ColourIds::buttonColourId, annuler);

	key[13].setButtonText("0");
	key[13].setColour(TextButton::ColourIds::buttonColourId, c);

	key[14].setButtonText("<<");
	key[14].onClick = [this]() {label.setText(label.getText().substring(0, label.getText().length() - 1), NotificationType::sendNotification); };
	key[14].setColour(TextButton::ColourIds::buttonColourId, retour);

	key[15].setButtonText("OK");
	key[15].onClick = [this]()
	{
		output = label.getText();
		if (output.length() == 0) output = "        ";

		if (!Price::isValid(output)) {
			label.setColour(Label::ColourIds::textColourId, Colours::red);
			label.repaint();
			Timer::callAfterDelay(220, [this]()
				{
					label.setColour(Label::ColourIds::textColourId, Colours::white);
					label.repaint();
				});
			return;
		}

		caller->setText(output, NotificationType::sendNotification);
		caller->textManuallyUpdated();
		setVisible(false);
	};
	key[15].setColour(TextButton::ColourIds::buttonColourId, confirmer);

	label.setFont(Font("Seven Segment", 30, 0));
	label.setColour(Label::ColourIds::backgroundColourId, Colours::black);
	addAndMakeVisible(label);
}

Keyboard::~Keyboard()
{
	removeKeyListener(this);
}

void Keyboard::resetAndShow(SpecialLabel* c, const String& startingText, unsigned int maxNumberOfCharacters)
{
	caller = c;
	label.setText(startingText, NotificationType::sendNotification);
	output = startingText;
	setVisible(true);
	if (label.getCurrentTextEditor())
		label.getCurrentTextEditor()->selectAll();
	currentMaxNumChar = maxNumberOfCharacters - 1;
}

void Keyboard::paint(juce::Graphics& g)
{
}

void Keyboard::resized()
{
	disable.setBounds(getLocalBounds());

	juce::Rectangle<int> keyboardArea;
	float ratio = 0.65, size = 0.65;
	keyboardArea.setCentre(getBounds().getCentre());
	keyboardArea = keyboardArea.withSizeKeepingCentre(jmin<int>(getWidth(), getHeight() * size * ratio), getHeight() * size);

	grid.setBounds(keyboardArea);
	for (int y = 0; y < NUM_ROWS; y++)
		for (int x = 0; x < NUM_COLUMNS; x++)
			key[y * NUM_ROWS + x].setBounds(grid.getRectangle(x, y, x + 1, y + 1, true));
	label.setBounds(grid.getLine(grid.getNumRows() - 1, true));
}

bool Keyboard::keyPressed(const KeyPress& k, Component* originatingComponent)
{
	char s[] = "A";
	for (char c = 32; c <= 125; c++)
		if (k.getTextCharacter() == c) {
			s[0] = c;
			String st(s);
			if (st.containsOnly(Core::get().getDigitEditorAcceptedCharacters())) {
				if (currentMaxNumChar == 0)
					label.setText(String(s), NotificationType::sendNotification);
				if (label.getText().length() <= currentMaxNumChar) {
					label.setText(label.getText(true) + st, NotificationType::sendNotification);
				}
				return true;
			}
		}

	if (k == KeyPress::backspaceKey)
		key[14].onClick();
	else if (k == KeyPress::escapeKey)
		key[11].onClick();
	else if (k == KeyPress::returnKey)
		key[15].onClick();

	return false;
}
//
//ApplicationCommandTarget* Keyboard::getNextCommandTarget()
//{
//	return nullptr;
//}
//
//void Keyboard::getAllCommands(Array<CommandID>& commands)
//{
//	Array<CommandID> ids{ KeyPressCommandIDs::b0, KeyPressCommandIDs::b1,
//							   KeyPressCommandIDs::b2, KeyPressCommandIDs::b3,
//							   KeyPressCommandIDs::b4, KeyPressCommandIDs::b5,
//							   KeyPressCommandIDs::b6, KeyPressCommandIDs::b7,
//							   KeyPressCommandIDs::b8, KeyPressCommandIDs::b9,
//	};
//
//	commands.addArray(ids);
//}
//
//void Keyboard::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
//{
//	switch (commandID)
//	{
//	case KeyPressCommandIDs::b0:
//		result.addDefaultKeypress(KeyPress::numberPad0, 0);
//		result.addDefaultKeypress(KeyPress::, 0);
//		break;
//	default:
//		break;
//	}
//}
//
//bool Keyboard::perform(const InvocationInfo& info)
//{
//	switch (info.commandID)
//	{
//	case KeyPressCommandIDs::buttonMoveUp:
//		DBG("Up");
//		resized();
//		break;
//	case KeyPressCommandIDs::buttonMoveRight:
//		DBG("R");
//
//		resized();
//		break;
//	case KeyPressCommandIDs::buttonMoveDown:
//		DBG("D");
//
//		resized();
//		break;
//	case KeyPressCommandIDs::buttonMoveLeft:
//		DBG("L");
//		resized();
//		break;
//	case KeyPressCommandIDs::nextButtonColour:
//		DBG("Up");
//		break;
//	case KeyPressCommandIDs::previousButtonColour:
//		DBG("Up");
//		break;
//	case KeyPressCommandIDs::nextBackgroundColour:
//		DBG("Up");
//		repaint();
//		break;
//	case KeyPressCommandIDs::previousBackgroundColour:
//		DBG("Up");
//		repaint();
//		break;
//	default:
//		return false;
//	}
//
//	return true;
//}
