/*
  ==============================================================================

	Buttons.cpp
	Created: 21 Jun 2021 12:37:46pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "Buttons.h"

//==============================================================================
Buttons::Buttons() : send("Initialiser"), stop("Stop"), grid(4, 1), progression(sendThread),
verif(CharPointer_UTF8("Vérifier tous les segments \n(OFF)")),
COMErrorWindow("Erreur lors de l'ouverture du port COM", "", AlertWindow::AlertIconType::WarningIcon)
{
	addAndMakeVisible(grid);
	addAndMakeVisible(verif);
	send.setLookAndFeel(Core::get().getLookAndFeel().get());
	stop.setLookAndFeel(Core::get().getLookAndFeel().get());
	verif.setLookAndFeel(Core::get().getLookAndFeel().get());

	send.setColour(TextButton::ColourIds::buttonColourId, lfColours::sendButton);
	stop.setColour(TextButton::ColourIds::buttonColourId, lfColours::stopButton);
	verif.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);

	stop.setEnabled(false);
	verif.setEnabled(false);

	addAndMakeVisible(send);
	addAndMakeVisible(stop);
	addAndMakeVisible(progression);

	COMErrorWindow.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));

	send.onClick = [this]()
	{
		if (sendThread.isThreadRunning())
			return;

		if (!Core::get().isInit()) {
			auto& c = Core::get();
			if (UART::checkCOMPort(Core::get().getCOMPort())) {
				Sequence s;
				auto& c = Core::get();
				Price prices[Core::MAX_PRICES];
				for (int i = 0; i < Core::MAX_PRICES; i++)
					prices[i] = c.getPrice(i);
				s.createSequence(c.getNumPrices(), c.getNumDigits(), prices, 50, true);
				sendThread.setSequence(s);
			}
			else {
				COMErrorWindow.setAlwaysOnTop(true);
				COMErrorWindow.enterModalState(true, ModalCallbackFunction::create([this](int r)
					{
						COMErrorWindow.setVisible(false);
					}
				), false);
			}
		}
		else
		{
			Sequence s;
			auto& c = Core::get();
			s = Core::get().createOptimizedSequence();
			sendThread.setSequence(s);
		}
		stop.setEnabled(true);
		verif.setEnabled(false);
		Core::get().setInTransmission(true);
		Core::get().updateVisualization();
		progression.start();
		sendThread.startThread();
	};

	stop.onClick = [this]()
	{
		sendThread.askToExit();
	};

	Core::get().sendSequence = [this]()
	{
		if (sendThread.isThreadRunning())
			return;

		stop.setEnabled(true);
		verif.setEnabled(false);
		Core::get().setInTransmission(true);
		Core::get().updateVisualization();
		sendThread.setSequence(Core::get().getSequence());
		progression.start();
		sendThread.startThread();
	};
	verif.setColour(TextButton::ColourIds::buttonColourId, lfColours::stopButton);
	verif.setColour(TextButton::ColourIds::buttonOnColourId, lfColours::sendButton);

	verif.setClickingTogglesState(true);
	verif.setToggleState(Core::get().getLineControl(), NotificationType::sendNotification);
	verif.onClick = [this]()
	{
		if (verif.getToggleState() == true) {
			verif.setButtonText(CharPointer_UTF8("Vérifier tous les segments \n(ON)"));
			Core::get().setLineControl(true);
		}
		else {
			verif.setButtonText(CharPointer_UTF8("Vérifier tous les segments \n(OFF)"));
			Core::get().setLineControl(false);
		}
	};
	verif.onClick();
}

Buttons::~Buttons()
{
	sendThread.askToExit();
	sendThread.stopThread(2500);
	sendThread.waitForThreadToExit(2500);
}

void Buttons::paint(juce::Graphics& g)
{
}

void Buttons::resized()
{
	grid.setBounds(getLocalBounds());
	send.setBounds(grid.getRectangle(1, 0, 3, 1));
	verif.setBounds(grid.getRectangle(3, 0, 4, 1));
	stop.setBounds(grid.getRectangle(0, 0, 1, 1));
	progression.setBounds(send.getBounds());
}

void Buttons::updateVizualisation()
{
	if (!Core::get().isInit()) {
		verif.setEnabled(false);
		send.setButtonText("Initaliser");
	}
	else {
		send.setButtonText("Envoyer");
		if (Core::get().getIsInTransmission())
			verif.setEnabled(false);
		else
			verif.setEnabled(true);
	}
	if (Core::get().getIsInTransmission())
		stop.setEnabled(true);
	else
		stop.setEnabled(false);
}

void Progression::start()
{
	setVisible(true);
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
	g.setColour(Colours::black);
	g.drawFittedText(String(int(p * 100)) + String("%"), getLocalBounds(), Justification::centred, 1);
}

void Progression::timerCallback()
{
	repaint();
	if (thread.getProgression() >= 0.999f) {
		stopTimer();
		setVisible(false);
	}
}
