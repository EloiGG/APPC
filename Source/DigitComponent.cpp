/*
  ==============================================================================

	DigitComponent.cpp
	Created: 17 Jun 2021 11:04:03am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DigitComponent.h"

//==============================================================================

DigitEditor::DigitEditor() : frameCounter(0), hasState(true)
{
	state.err_ok = true;
	state.work_in_progress = true;
	setEditable(true);
	setRepaintsOnMouseActivity(true);
	setOpaque(true);
	setJustificationType(Justification::centred);
	setFont(Font("Seven Segment", "Regular", getHeight()));
	setColour(Label::ColourIds::textColourId, lfColours::digitColour);
}

void DigitEditor::paint(juce::Graphics& g)
{
	auto c = isMouseOver() ? lfColours::digitBackground.brighter() : lfColours::digitBackground;
	g.fillAll(c);
	g.setColour(lfColours::digitColour);
	Label::paint(g);
	g.setColour(Colours::white);
	g.drawRect(getLocalBounds());

	float updateRectangleMargin = getWidth() / 10, cornerSize = getHeight() / 40.0f;
	if (frameCounter > 0) {
		g.setColour(lfColours::onDigitUpdate.withAlpha(jmap<float>(frameCounter, 0, 60, 1, 0)));
		g.drawRoundedRectangle(getLocalBounds().reduced(updateRectangleMargin).toFloat(), cornerSize, 1.0f);
	}

	if (!hasState) return;
	Colour clr;
	if (state.err_ok) clr = lfColours::errModuleOK;
	else if (state.work_in_progress || state.stopping) clr = lfColours::errModuleWIP;
	else if (!state.upToDate) clr = lfColours::upToDate;
	else if (state.erreurs[state.err_illisible] || state.erreurs[state.err_reponse]) clr = lfColours::errModuleTimeout;
	else clr = lfColours::errModuleSegment;
	g.setColour(clr);
	int border = getWidth() * 0.2;
	juce::Rectangle<float> rect(getWidth() - border * 1.5, border * 0.5, border, border);
	g.fillEllipse(rect);
}

void DigitEditor::setDigit(const String& newDigit)
{
	setText(newDigit.substring(0, 1), NotificationType::sendNotification);
	frameCounter = 0;
	startTimerHz(60);
	timerCallback();
}

String DigitEditor::getDigit()
{
	return getText().substring(0, 1);
}

void DigitEditor::resized()
{
	Label::resized();
	auto f = getFont();
	f.setHeight(getHeight());
	setFont(f);
}

void DigitEditor::setState(const ErrModule& newState)
{
	state = newState;
	//DBG("changement : uptodate = " << (int)newState.upToDate);
	String tip;
	if (state.err_ok) tip = (L"Pas d'erreurs détectées sur ce module");
	else if (state.work_in_progress || state.stopping) tip = (L"Pas d'informations sur l'état du module");
	else if (!state.upToDate) tip = (L"Le prix n'a pas encore été envoyé vers le panneau");
	else if (state.erreurs[state.err_illisible]) tip = (L"La réponse du module n'a pas pu être comprise");
	else if (state.erreurs[state.err_reponse]) tip = (L"Le module n'a pas fourni de réponse");
	else {
		tip = CharPointer_UTF8("Erreurs sur les segments suivants : ");
		if (state.erreurs[state.err_A]) tip += "A";
		if (state.erreurs[state.err_B]) tip += "B";
		if (state.erreurs[state.err_C]) tip += "C";
		if (state.erreurs[state.err_D]) tip += "D";
		if (state.erreurs[state.err_E]) tip += "E";
		if (state.erreurs[state.err_F]) tip += "F";
		if (state.erreurs[state.err_G]) tip += "G";
	}
	setTooltip(tip);
	repaint();
}

void DigitEditor::setShowState(bool shouldShowState)
{
	hasState = shouldShowState;
}

void DigitEditor::updateAnimation()
{
	startTimerHz(60);
	timerCallback();
}

void DigitEditor::textKeyboardUpdated()
{
	state.upToDate = false;
	state.work_in_progress = false;
	setState(state);
	setDigit(getText());
}

void DigitEditor::timerCallback()
{
	repaint();
	if (++frameCounter > 60) {
		frameCounter = 0;
		stopTimer();
	}
}
