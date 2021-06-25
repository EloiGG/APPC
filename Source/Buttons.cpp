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
loadConfigButton("Charger une configuration"), connectButton(L"Se connecter au réseau"),
connectWindow(L"Se connecter à CentoFuel", "Veuillez entrer votre identifiant", AlertWindow::AlertIconType::QuestionIcon),
networkErrorWindow(L"Erreur réseau", "", AlertWindow::AlertIconType::WarningIcon),
networkSuccessWindow(L"Connexion à CentoFuel réussie", "Charger les informations sur le panneau ?", AlertWindow::AlertIconType::QuestionIcon),
configSuccessWindow(L"Chargement de la configuration réussie", "", AlertWindow::AlertIconType::InfoIcon),
filechooser(L"Sélectionner un fichier de config", File::getCurrentWorkingDirectory().getChildFile("init.config"), String("*.config"))
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

	configSuccessWindow.addButton("Ok", 1, KeyPress(KeyPress::returnKey, 0, 0));


	if (File::getCurrentWorkingDirectory().getChildFile("init.config").existsAsFile()) {
		auto& c = Core::get();
		c.setJSON(File::getCurrentWorkingDirectory().getChildFile("init.config"));
		c.loadInformationsFromJSON();
		if (c.hasNetwork()) {
			networkWindows(c.getNetwork());
		}
	}

	loadConfigButton.onClick = [this]()
	{
		if (filechooser.browseForFileToOpen()) {
			auto& c = Core::get();
			c.setJSON(filechooser.getResult()); 
			c.loadInformationsFromJSON();
			configSuccessWindow.enterModalState(true, ModalCallbackFunction::create([this, &c](int r)
				{
					if (r == 1) {
						configSuccessWindow.setVisible(false);
						if (c.hasNetwork())
							networkWindows(c.getNetwork(), false);
					}
					else
						configSuccessWindow.setVisible(false);
				}
			));
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
					networkWindows(net);
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

void Buttons::networkWindows(const Network& net, bool retry)
{
	if (net.getPassword() == "")
		return;
	auto connected = net.connected();
	if (std::get<0>(connected)) {
		Core::get().setNetwork(net);
		networkSuccessWindow.setAlwaysOnTop(true);
		networkSuccessWindow.enterModalState(true, ModalCallbackFunction::create([this](int r)
			{
				if (r == 1)
					Core::get().loadInformationsFromNetwork();
				networkSuccessWindow.setVisible(false);
			}
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
		networkErrorWindow.setAlwaysOnTop(true);
		networkErrorWindow.enterModalState(true, ModalCallbackFunction::create([this, retry](int r)
			{
				networkErrorWindow.setVisible(false);
				if (retry)
					connectButton.onClick();
			}
		), false);
	}
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
	g.drawFittedText(String(int(p * 100)) + String("%"), getLocalBounds(), Justification::centred, 1);
}

void Progression::timerCallback()
{
	repaint();
	if (thread.getProgression() >= 0.999f) {
		stopTimer();
	}
}
