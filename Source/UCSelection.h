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
#include "Core.h"
//==============================================================================
/*
*/
class UCSelectionDialogBox : public DialogBoxComponent
{
	class UCSelection : public CustomTableListBox
	{
	public:
		UCSelection();
		~UCSelection() override;

		virtual void cellClicked(int rowNumber, int columnId, const MouseEvent&) override;
		virtual String getPropriety(int row, int column) override;
		void paint(Graphics&) override;

	private:
		UCsJSON UCsjson;
		int currentUCID;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UCSelection)
	};

public:

	void open() override;
	UCSelectionDialogBox();
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UCSelectionDialogBox)

};