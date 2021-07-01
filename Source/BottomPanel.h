/*
  ==============================================================================

	BottomPanel.h
	Created: 19 Jun 2021 1:08:30am
	Author:  Eloi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"
#include "Core.h"
#include "InputComponent.h"
//==============================================================================
/*
*/
class BottomPanel : public juce::Component
{
public:
	BottomPanel();
	~BottomPanel() override;

	void paint(juce::Graphics&) override;
	void resized() override;
	void resetText();
	TextEditor* getTextEditor();

private:
	TextButton save;
	TextEditor t;
	CheckBox details;
	FileChooser filechooser;
	int debuggingLevel = 1;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BottomPanel)

};
