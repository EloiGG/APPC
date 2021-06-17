/*
  ==============================================================================

    PriceComponent.cpp
    Created: 17 Jun 2021 9:16:21am
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PriceComponent.h"
#include "LookAndFeel.h"

//==============================================================================
PriceComponent::PriceComponent(int number_of_digits) : numDigits(number_of_digits), grid(number_of_digits, 1)
{
    setRepaintsOnMouseActivity(false);
    addAndMakeVisible(priceEditor);
    for (int i = 0; i < numDigits; i++)
        addAndMakeVisible(digits[i]);
    priceEditor.setFont(priceEditorFont);
    
    priceEditor.setPrice("1223");
    priceEditor.setJustificationType(Justification::centred);
    priceEditorFont = Font("Seven Segment", "Regular", getHeight());
    

}

PriceComponent::~PriceComponent()
{
}


void PriceComponent::paint (juce::Graphics& g)
{
    int c = 25;
    g.setColour(Colour(c, c, c));
    g.fillAll();
    
}


void PriceComponent::setNumberOfDigits(int number_of_digits)
{
    grid.resize(number_of_digits, grid.getNumRows());
    numDigits = number_of_digits;
}

void PriceComponent::mouseDoubleClick(const MouseEvent& event)
{
    priceEditor.setVisible(false);
    if (priceEditor.isBeingEdited())
        DBG("edit");
}


void PriceComponent::resized()
{
    grid.setBounds(getLocalBounds());
    float whRatio = 4.0f / 5.0f;
    for (int i = 0; i < numDigits; i++) {
        auto rect = grid.getRectangle(i, 0, i + 1, 1);
        auto rect2 = rect.withWidth(rect.getHeight() * whRatio).withCentre(rect.getCentre());
        digits[i].setBounds(rect2);
    }
    priceEditor.setBounds(getLocalBounds());
    priceEditorFont.setHeight(getHeight());
    priceEditor.setFont(priceEditorFont);
}

PriceEditor::PriceEditor()
{
    setOpaque(true);
    setEditable(true);
    setBroughtToFrontOnMouseClick(true);
    setJustificationType(Justification::centred);
    setRepaintsOnMouseActivity(true);
    setText("333", NotificationType::sendNotification);
    setColour(Label::ColourIds::textColourId, findColour(Label::ColourIds::textColourId).withAlpha(0.0f));
}

void PriceEditor::setPrice(const String& newPrice)
{
    setText(newPrice, NotificationType::sendNotification);
}

void PriceEditor::paint(juce::Graphics& g)
{
    auto c = isMouseOverOrDragging(true) ? lfColours::priceBackground.brighter() : lfColours::priceBackground;
    g.fillAll(c);
    Label::paint(g);
}

void PriceEditor::mouseDown(const MouseEvent& m)
{
    setColour(Label::ColourIds::textColourId, findColour(Label::ColourIds::textColourId).withAlpha(1.0f));
    showEditor();
}

void PriceEditor::editorAboutToBeHidden(TextEditor*)
{
    setColour(Label::ColourIds::textColourId, findColour(Label::ColourIds::textColourId).withAlpha(0.0f));
    setAlwaysOnTop(false);
    toBack();
}
