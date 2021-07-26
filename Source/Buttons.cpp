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

	addAndMakeVisible(send);
	addAndMakeVisible(stop);
	addAndMakeVisible(progression);

	COMErrorWindow.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));

	send.onClick = [this]()
	{
		if (!Core::get().isInit()) {
			auto& c = Core::get();
			if (UART::checkCOMPort(Core::get().getCOMPort())) {
		//		for (int i = 0; i < Core::MAX_PRICES; i++)
		//			Core::get().setPrice(i, Price("8.88888"));
		//		Price prices[Core::MAX_PRICES];
		//		for (int i = 0; i < Core::MAX_PRICES; i++)
		//			prices[i] = c.getPrice(i);
		//		s.createSequence(c.getNumPrices(), c.getNumDigits(), prices, 50, true);
		//		sendThread.setSequence(s);
		//		Core::get().updateVisualization();
		//		for (int i = 0; i < c.getNumPrices(); i++)
		//			Core::get().updatePrices(TextUpdateOrigin::Omni, i);
		//Core::get().setInTransmission(true);
		//Core::get().updateVisualization();
		//		progression.start();
		//		sendThread.startThread();
		//		stop.setEnabled(true);
		//		return;
				Sequence s;
				auto& c = Core::get();
				Price prices[Core::MAX_PRICES];
				for (int i = 0; i < Core::MAX_PRICES; i++)
					prices[i] = c.getPrice(i);
				s.createSequence(c.getNumPrices(), c.getNumDigits(), prices, c.getDelay(), c.getLineControl());
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
		if (sendThread.isThreadRunning())
			return;

		if (Core::get().getPlaySequence() == false)
		{
			Sequence s;
			auto& c = Core::get();
			Price prices[Core::MAX_PRICES];
			for (int i = 0; i < Core::MAX_PRICES; i++)
				prices[i] = c.getPrice(i);
			s.createSequence(c.getNumPrices(), c.getNumDigits(), prices, c.getDelay(), c.getLineControl());
			sendThread.setSequence(s);
		}
		else
			sendThread.setSequence(Core::get().getSequence());
		stop.setEnabled(true);
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
		Core::get().setInTransmission(true);
		Core::get().updateVisualization();
		sendThread.setSequence(Core::get().getSequence());
		progression.start();
		sendThread.startThread();
	};
	verif.setColour(TextButton::ColourIds::buttonColourId, lfColours::stopButton);
	verif.setColour(TextButton::ColourIds::buttonOnColourId, lfColours::sendButton);

	verif.setClickingTogglesState(true);
	verif.onClick = [this]()
	{
		if (verif.getToggleState() == true)
			verif.setButtonText(CharPointer_UTF8("Vérifier tous les segments \n(ON)"));
		else
			verif.setButtonText(CharPointer_UTF8("Vérifier tous les segments \n(OFF)"));
	};
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
		stop.setEnabled(false);
		verif.setEnabled(false);
		send.setButtonText("Initaliser");
	}
	else {
		stop.setEnabled(true);
		verif.setEnabled(true);
		send.setButtonText("Envoyer");
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
