/*
  ==============================================================================

	Buttons.cpp
	Created: 21 Jun 2021 12:37:46pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "Buttons.h"

//==============================================================================
Buttons::Buttons() : send("Envoyer"), stop("Stop"), grid(4, 2), progression(sendThread),
loadConfigButton("Charger une configuration"), connectButton("Se connecter au reseau"),
connectWindow("Se connecter a CentoFuel", "Veuillez entrer votre identifiant", AlertWindow::AlertIconType::QuestionIcon),
networkErrorWindow("Erreur reseau", "", AlertWindow::AlertIconType::WarningIcon),
networkSuccessWindow("Connexion reussie", "Charger les informations sur le panneau ?", AlertWindow::AlertIconType::QuestionIcon),
filechooser("test", File::getCurrentWorkingDirectory().getChildFile("test.config"), String("*.config"))
{
	addAndMakeVisible(grid);

	send.setLookAndFeel(Core::get().getLookAndFeel().get());
	stop.setLookAndFeel(Core::get().getLookAndFeel().get());

	send.setColour(TextButton::ColourIds::buttonColourId, lfColours::sendButton);
	stop.setColour(TextButton::ColourIds::buttonColourId, lfColours::stopButton);

	stop.setEnabled(false);

	addAndMakeVisible(send);
	addAndMakeVisible(stop);
	addAndMakeVisible(connectButton);
	addAndMakeVisible(loadConfigButton);
	addAndMakeVisible(progression);
	connectButton.setLookAndFeel(Core::get().getLookAndFeel().get());
	connectButton.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);
	loadConfigButton.setLookAndFeel(Core::get().getLookAndFeel().get());
	loadConfigButton.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);

	connectWindow.addTextEditor("MDP", "", "Mot de passe", true);
	connectWindow.addButton("Se connecter", 1, KeyPress(KeyPress::returnKey, 0, 0));
	connectWindow.addButton("Annuler", 2, KeyPress(KeyPress::escapeKey, 0, 0));
	connectWindow.setEscapeKeyCancels(true);

	networkSuccessWindow.addButton("Oui", 1, KeyPress(KeyPress::returnKey, 0, 0));
	networkSuccessWindow.addButton("Non", 2, KeyPress(KeyPress::escapeKey, 0, 0));
	networkErrorWindow.addButton("Fermer", 0);

	loadConfigButton.onClick = [this]()
	{
		if (filechooser.browseForFileToOpen()) {
			Core::get().setJSON(ConfigJSON(filechooser.getResult()));
			Core::get().loadInformationsFromJSON();
		}
	};

	connectButton.onClick = [this]()
	{
		connectWindow.enterModalState(true, ModalCallbackFunction::create([this](int r)
			{
				if (r == 1)
				{
					auto text = connectWindow.getTextEditorContents("MDP");
					Network net("X-AUTH-TOKEN", text);
					auto connected = net.connected();
					if (std::get<0>(connected)) {
						Core::get().setNetwork(net);
						networkSuccessWindow.enterModalState(true, ModalCallbackFunction::create([this](int r)
							{if (r == 1)Core::get().loadInformationsFromNetwork(); networkSuccessWindow.setVisible(false); }
						), false);
					}
					else {
						String errorMessage;
						switch (std::get<1>(connected)) // code erreur	
						{
						case 404:
							errorMessage = "Resource not found";
							break;
						case 403:
							errorMessage = "Access denied";
							break;
						case 401:
							errorMessage = "Authentication problems";
							break;
						case 400:
							errorMessage = "Bad Type";
							break;
						default:
							break;
						}
						networkErrorWindow.setMessage(String("Message d'erreur : \n\"") + errorMessage + String("\"\n")
							+ String("Code erreur : ") + String(std::get<1>(connected)) + String("\n"));
						networkErrorWindow.enterModalState(true, ModalCallbackFunction::create([this](int r)
							{networkErrorWindow.setVisible(false); connectButton.onClick(); }
						), false);
					}
				}
				connectWindow.setVisible(false);
			}), false);
	};

	send.onClick = [this]()
	{
		if (sendThread.isThreadRunning())
			return;
		Sequence s;
		s.createSequence(Core::get());
		sendThread.setSequence(s);
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
}

Buttons::~Buttons()
{
	sendThread.stopThread(1500);
}

void Buttons::paint(juce::Graphics& g)
{
}

void Buttons::resized()
{
	grid.setBounds(getLocalBounds());
	connectButton.setBounds(grid.getRectangle(0, 0, 2, 1));
	loadConfigButton.setBounds(grid.getRectangle(2, 0, 4, 1));
	send.setBounds(grid.getRectangle(1, 1, 4, 2));
	stop.setBounds(grid.getRectangle(0, 1, 1, 2));
	progression.setBounds(send.getBounds());
}

void Buttons::updateVizualisation()
{
	if (Core::get().getIsInTransmission())
		stop.setEnabled(true);
	else
		stop.setEnabled(false);
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
	g.setColour(Colours::black);
	g.drawFittedText(String(int(p*100)) + String("%"), getLocalBounds(), Justification::centred, 1);
}

void Progression::timerCallback()
{
	repaint();
	if (thread.getProgression() >= 0.999f) {
		stopTimer();
	}
}
