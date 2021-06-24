/*
  ==============================================================================

	Buttons.cpp
	Created: 21 Jun 2021 12:37:46pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "Buttons.h"

//==============================================================================
Buttons::Buttons() : send("Envoyer"), stop("Stop"), grid(4, 1), progression(sendThread)
{
	addAndMakeVisible(grid);

	send.setLookAndFeel(Core::get().getLookAndFeel().get());
	stop.setLookAndFeel(Core::get().getLookAndFeel().get());

	send.setColour(TextButton::ColourIds::buttonColourId, lfColours::sendButton);
	stop.setColour(TextButton::ColourIds::buttonColourId, lfColours::stopButton);

	stop.setEnabled(false);

	addAndMakeVisible(send);
	addAndMakeVisible(stop);
	addAndMakeVisible(progression);

	send.onClick = [this]()
	{
		if (sendThread.isThreadRunning())
			return;
		Sequence s;
		s.createSequence(Core::get());
		sendThread.setSequence(s);
		stop.setEnabled(true);
		progression.start();
		sendThread.startThread();
	};

	stop.onClick = [this]() {sendThread.stopThread(2500); };
}

Buttons::~Buttons()
{
	sendThread.stopThread(1500);
}

void Buttons::paint(juce::Graphics& g)
{
	/*g.setColour(Colours::antiquewhite);
	g.fillRect(send.getBounds().withWidth(jmap<float>(sendThread.getProgression(), 0, getWidth())).toFloat());*/
}

void Buttons::resized()
{
	float separation = 0.2;
	grid.setBounds(getLocalBounds());
	send.setBounds(grid.getRectangle(1, 0, 4, 1));
	stop.setBounds(grid.getRectangle(0, 0, 1, 1));
	progression.setBounds(send.getBounds());
}


void Progression::start()
{
	startTimerHz(60);
}

void Progression::paint(juce::Graphics& g)
{
	if (!thread.isThreadRunning())
		return;

	auto p = thread.getProgression();
	g.fillAll(lfColours::sendButton);
	g.setColour(Colours::white.withAlpha((float)0.8));
	if (p <= 0.999f)
		g.fillRect(getLocalBounds().withWidth(jmap<float>(thread.getProgression(), 0, getWidth())).toFloat());
}

void Progression::timerCallback()
{
	repaint();
	if (thread.getProgression() >= 0.999f) {
		DBG("fin");
		stopTimer();
	}
}
