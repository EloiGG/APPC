/*
  ==============================================================================

	UCSelection.h
	Created: 22 Jul 2021 3:47:05pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DialogBox.h"

//==============================================================================
/*
*/
class UCSelectionDialogBox : public DialogBoxComponent
{
	class UCSelection : public juce::Component
	{
	public:
		UCSelection();
		~UCSelection() override;

		void paint(juce::Graphics&) override;
		void resized() override;

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UCSelection)
	};

public:
	UCSelectionDialogBox();
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UCSelectionDialogBox)

};