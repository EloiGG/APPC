/*
  ==============================================================================

	Buttons.h
	Created: 21 Jun 2021 12:37:46pm
	Author:  Eloi GUIHARD-GOUJON

	Classes nécessaires pour les boutons en bas du panneau principal

  ==============================================================================
*/

#pragma once

#include "LookAndFeel.h"
#include "GridTool.h"
#include "Core.h"
#include "SerialThread.h"
#include "Sequence.h"
#include "Log.h"

//==============================================================================
/*
*/

/// <summary>
/// Component type barre de progrès affichant l'avancement d'un ProgressionThread
/// </summary>
class Progression : public Component, private Timer
{
public:
	Progression(const ProgressionThread& t) : thread(t) { setInterceptsMouseClicks(false, false); }

	// Commencer le thread
	void start();

	void paint(juce::Graphics&) override;

private:
	virtual void timerCallback() override;
	const ProgressionThread& thread;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Progression)
};

/// <summary>
/// Component comprenant les 3 boutons en bas du panneau principal (send, stop et verif)
/// </summary>
class Buttons : public juce::Component
{
public:
	Buttons();
	~Buttons() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	// Mettre à jour l'affichage 
	void updateVizualisation();

private:
	AlertWindow COMErrorWindow;
	SerialThread sendThread;
	TextButton send, stop, verif;
	GridTool grid;
	Progression progression;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Buttons)
};