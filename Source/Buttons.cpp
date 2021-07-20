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
loadConfigButton("Charger une configuration"), connectButton(CharPointer_UTF8("Se connecter au réseau")),
connectWindow(CharPointer_UTF8("Se connecter à CentoFuel"), "Veuillez entrer votre identifiant", AlertWindow::AlertIconType::QuestionIcon),
networkErrorWindow(CharPointer_UTF8("Erreur réseau"), "", AlertWindow::AlertIconType::WarningIcon),
networkSuccessWindow(CharPointer_UTF8("Connexion à CentoFuel réussie"), "Charger les informations sur le panneau ?", AlertWindow::AlertIconType::QuestionIcon),
configSuccessWindow(CharPointer_UTF8("Chargement de la configuration réussie"), "", AlertWindow::AlertIconType::InfoIcon),
filechooser(CharPointer_UTF8("Sélectionner un fichier de config"), File::getCurrentWorkingDirectory().getChildFile("init.config"), String("*.config")),
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
	//addAndMakeVisible(connectButton);
	//addAndMakeVisible(loadConfigButton);
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

	COMErrorWindow.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));


	if (File::getCurrentWorkingDirectory().getChildFile("lastconfig.config").existsAsFile()) {
		Log::write("Chargement du fichier lastconfig.config");
		Log::ln();
		auto& c = Core::get();
		c.setConfigJSON(File::getCurrentWorkingDirectory().getChildFile("lastconfig.config"));
		c.loadInformationsFromJSON();
		if (c.hasNetwork()) {
			networkWindows(c.getNetwork());
		}
	}

	loadConfigButton.onClick = [this]()
	{
		if (filechooser.browseForFileToOpen()) {
			auto& c = Core::get();
			c.setConfigJSON(filechooser.getResult());
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
		/*if (!Core::get().isInit()) {
			Sequence s;
			auto& c = Core::get();
			if (UART::checkCOMPort(Core::get().getCOMPort())) {
				for (int i = 0; i < Core::MAX_PRICES; i++)
					Core::get().setPrice(i, Price("8.88888"));
				Price prices[Core::MAX_PRICES];
				for (int i = 0; i < Core::MAX_PRICES; i++)
					prices[i] = c.getPrice(i);
				s.createSequence(c.getNumPrices(), c.getNumDigits(), prices, 50, true);
				sendThread.setSequence(s);
				Core::get().updateVisualization();
				for (int i = 0; i < c.getNumPrices(); i++)
					Core::get().updatePrices(TextUpdateOrigin::Omni, i);
				progression.start();
				sendThread.startThread();
				stop.setEnabled(true);
				return;
			}
			else {
				COMErrorWindow.setAlwaysOnTop(true);
				COMErrorWindow.enterModalState(true, ModalCallbackFunction::create([this](int r)
					{
						COMErrorWindow.setVisible(false);
					}
				), false);
			}
		}*/

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
	//connectButton.setBounds(grid.getRectangle(0, 0, 2, 1));
	//loadConfigButton.setBounds(grid.getRectangle(2, 0, 4, 1));
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

void Buttons::networkWindows(const Network& net, bool retry)
{
	Core::get().setNetwork(net);
	if (net.getPassword() == "") {
		Log::write("Pas de mot de passe fourni !\n\n");
		return;
	}
	auto connected = net.connected();
	if (std::get<0>(connected)) {
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
	Log::ln(1, 2);
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
