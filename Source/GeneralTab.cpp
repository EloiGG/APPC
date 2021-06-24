/*
  ==============================================================================

	GeneralTab.cpp
	Created: 19 Jun 2021 10:42:19am
	Author:  Eloi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GeneralTab.h"

//==============================================================================
GeneralTab::GeneralTab() : grid(4, 12), nPrices("Nombre de prix", Core::get().getNumPrices()), nDigits("Nombre de chiffres", Core::get().getNumDigits()),
loadConfigButton("Charger une configuration"), connectButton("Se connecter au reseau"),
connectWindow("Se connecter a CentoFuel", "Veuillez entrer votre identifiant", AlertWindow::AlertIconType::QuestionIcon),
networkErrorWindow("Erreur reseau", "", AlertWindow::AlertIconType::WarningIcon),
networkSuccessWindow("Connexion reussie", "Charger les informations sur le panneau ?", AlertWindow::AlertIconType::QuestionIcon),
lineControl("Controle des segments"), resetLine("Effacer si erreur"), delay("Delai d'affichage (ms)", 50)
{
	addAndMakeVisible(nPrices);
	addAndMakeVisible(nDigits);
	addAndMakeVisible(delay);
	addChildComponent(grid);
	addAndMakeVisible(connectButton);
	addAndMakeVisible(loadConfigButton);
	addAndMakeVisible(lineControl);
	addAndMakeVisible(resetLine);

	loadConfigButton.setLookAndFeel(Core::get().getLookAndFeel().get());
	connectButton.setLookAndFeel(Core::get().getLookAndFeel().get());
	loadConfigButton.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);
	connectButton.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);

	//connectWindow.addTextEditor("ID", "Identifiant", "Identifiant", false);
	connectWindow.addTextEditor("MDP", "", "Mot de passe", true);
	connectWindow.addButton("Se connecter", 1, KeyPress(KeyPress::returnKey, 0, 0));
	connectWindow.addButton("Annuler", 2, KeyPress(KeyPress::escapeKey, 0, 0));
	connectWindow.setEscapeKeyCancels(true);

	networkSuccessWindow.addButton("Oui", 1, KeyPress(KeyPress::returnKey, 0, 0));
	networkSuccessWindow.addButton("Non", 2, KeyPress(KeyPress::escapeKey, 0, 0));

	networkErrorWindow.addButton("Fermer", 0);

	grid.setBounds(0, 0, getWidth(), 300);

	nPrices.onUpdate = [](const String& input)
	{
		Core::get().setNumPrices(input.getIntValue());
		Core::get().updateVisualization();
	};
	nPrices.min = 1;
	nPrices.max = Core::MAX_PRICES;

	nDigits.onUpdate = [](const String& input)
	{
		Core::get().setNumDigits(input.getIntValue());
		Core::get().updateVisualization();
	};
	nDigits.min = 1;
	nDigits.max = Core::MAX_DIGITS;

	delay.onUpdate = [](const String& input)
	{
		Core::get().setDelay_ms(input.getIntValue());
	};
	delay.min = 0;
	delay.max = 20000;
	delay.increment = 50;
	delay.decrement = 50;

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
	lineControl.onStateChange = [this]() {Core::get().setLineControl(lineControl.getState()); };
	resetLine.onStateChange = [this]() {Core::get().setResetLine(resetLine.getState()); };
}

GeneralTab::~GeneralTab()
{
}

void GeneralTab::paint(juce::Graphics& g)
{
	g.fillAll(lfColours::tabBackground);
}

void GeneralTab::resized()
{
	grid.setBounds(grid.getLocalBounds().withWidth(getWidth()));
	nPrices.setBounds(grid.getRectangle(0, 0, 4, 1));
	nDigits.setBounds(grid.getRectangle(0, 1, 4, 2));
	lineControl.setBounds(grid.getRectangle(0, 2, 4, 3));
	resetLine.setBounds(grid.getRectangle(0, 3, 4, 4));
	delay.setBounds(grid.getRectangle(0, 4, 4, 5));
	connectButton.setBounds(grid.getRectangle(0, 10, 2, 12));
	loadConfigButton.setBounds(grid.getRectangle(2, 10, 4, 12));
}

void GeneralTab::setNumPrices(int newNumPrices)
{
	nPrices.setInput(String(newNumPrices));
}

void GeneralTab::setNumDigits(int newNumDigits)
{
	nDigits.setInput(String(newNumDigits));
}

void GeneralTab::updateAllParameters()
{
	auto& c = Core::get();
	setNumPrices(c.getNumPrices());
	setNumDigits(c.getNumDigits());
}
