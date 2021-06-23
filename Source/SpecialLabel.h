/*
  ==============================================================================

    SpecialLabel.h
    Created: 23 Jun 2021 9:27:16am
    Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <functional>

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
            textManuallyUpdated();
            lastText = getText();
        }
    }
    String lastText;
};