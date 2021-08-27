/*
  ==============================================================================

	Core.h
	Created: 19 Jun 2021 11:11:52am
	Author:  Eloi GUIHARD-GOUJON

	Contient le coeur du logiciel avec toutes les variables uniques qui sont utilisées par plusieurs composants du logiciel.
	Contient aussi des fonctions lambda qui peuvent être appelées par tous les composants du logiciel

  ==============================================================================
*/

#pragma once

#include "Price.h"
#include <functional>
#include "LookAndFeel.h"
#include "Networking.h"
#include "JSON.h"
#include "Sequence.h"
#include "SpecialLabel.h"
#include "AlertWindows.h"


/// <summary>
/// Struct résumant l'état d'un module 7 segments (en erreur ou pas, à jour, etc...)
/// </summary>
struct ErrModule
{
	// Renvoie une erreur de type pastille blanche
	static ErrModule white() { ErrModule r; r.work_in_progress = true; r.upToDate = true; return r; }

	// Renvoie une erreur type non à jour
	static ErrModule notUpToDate() { ErrModule r; r.upToDate = false; return r; }

	bool err_ok = 0;				// Sans erreur
	bool work_in_progress = 0;		// En travail
	bool stopping = 0;				// A été arrêté en plein travail 
	bool upToDate = 1;				// A jour
	bool erreurs[9] = { 0 };		// Tableau de booléen pour contenir les différentes erreurs possibles

	enum {
		err_A = 0,
		err_B = 1,
		err_C = 2,
		err_D = 3,
		err_E = 4,
		err_F = 5,
		err_G = 6,
		err_reponse = 7,
		err_illisible = 8
	};
	// Exemple : si erreurs[err_A] == true, alors il y a une erreur sur le segment A
};

// Enum pour savoir qui demande la mise à jour du texte
enum class TextUpdateOrigin
{
	PriceEditor, DigitEditor, InputComponent, Omni
};

/// <summary>
/// Classe coeur du programme (singleton). Contient les informations suivantes :
/// nombre de prix, nombre de chiffres par prix, ID de la station, ID du panneau, port COM.
/// Contient aussi plusieurs objets globaux au programme : 
/// tous les prix (type Price), accès au réseau (objet Network), fenêtres d'alerte
/// </summary>
class Core
{
public:
	//==============================================================================

	// Maximum de chiffres par prix
	static constexpr size_t MAX_DIGITS = 6;

	// Maximum de prix sur le panneau
	static constexpr size_t MAX_PRICES = 10;

	// Destruction du constructeur de copie : ne peut être dupliqué
	Core(const Core&) = delete;
	~Core() {  }

	// Libère la mémoire (appelé avant le déstructeur pour ne pas déclencher les alertes de fuite mem.)
	void kill() { delete configjson; delete pricesjson;  }

	// Retourne une référence vers le coeur du programme
	static Core& get();

	//==============================================================================

	unsigned int getNumDigits();
	void setNumDigits(unsigned int newNumDigits);

	unsigned int getNumPrices();
	void setNumPrices(unsigned int newNumPrices);

	Price getPrice(unsigned int index);
	void setPrice(unsigned int index, const Price& newPrice);

	unsigned int getDelay();
	void setDelay_ms(unsigned int newDelay);

	bool getLineControl() { return lineControl; }
	void setLineControl(bool newLineControl) { lineControl = newLineControl; }

	bool getResetLine() { return resetLine; }
	void setResetLine(bool newResetLine) { resetLine = newResetLine; }

	int getCurrentStationID() { return stationID; }
	void setCurrentStationID(int newStationID) { stationID = newStationID; }

	int getCurrentPanelID() { return panelID; }
	void setCurrentPanelID(int newPanelID) { panelID = newPanelID; }

	bool getIsInTransmission();
	void setInTransmission(bool shouldBeInTransmission);

	String getUCName() { return UCName; }
	void setUCName(const String& newName) { UCName = newName; }

	int getID() { return id; }
	void setID(int newID) { id = newID; }

	unsigned int getCOMPort() { return COM; }
	void setCOMPort(unsigned int newComPort) { COM = newComPort; }

	Network getNetwork() const;
	void setNetwork(const Network& net);

	void setPlaySequence(bool shouldPlaySequence) { playSequence = shouldPlaySequence; }
	bool getPlaySequence() { return playSequence; }

	bool isConnected() { return connected; }
	void setConnected(bool shouldBeConnected) { connected = shouldBeConnected; }

	bool getInTesting() { return inTesting; }
	void setInTesting(bool shouldBeInTesting) { inTesting = shouldBeInTesting; }

	bool isInSelection() { return inSelection; }
	void setInSelection(bool shouldBeInSelection) { inSelection = shouldBeInSelection; }

	//==============================================================================

	// Tente de se connecter au réseau précedemment choisi
	void tryToConnect() { if (networkInit) setNetwork(network); }

	// Retourne vrai si le réseau a été initialisé
	bool hasNetwork();

	// Choisir un fichier .config
	void setConfigJSON(const File& f) { if (configjson != nullptr) delete configjson; configjson = new ConfigJSON(f); }

	// Sauvegarder un fichier .config
	void saveConfigJSON(const File& f);

	// Sauvegarder un fichier .prices
	void savePriceSave(const File& f);

	// Met à jour le visuel si une connexion est établie
	void loadInformationsFromNetwork();

	// Charge les informations depuis le fichier .config sélectionné avec la méthode setConfigJSON
	void loadInformationsFromJSON();

	// Choisir la séquence de prix à afficher
	void setSequence(const Sequence& newSequence);
	Sequence getSequence() { return sequence; }
	void setSequenceDelay(unsigned int newDelay) { sequence.setDelay(newDelay); }

	std::shared_ptr<APPCLookAndFeel> getLookAndFeel();


	//==============================================================================

	//============================= FONCTIONS LAMBDA ===============================

	void setUpdatePriceFunction(const std::function<void(TextUpdateOrigin, unsigned int)>& f);

	// Met à jour les prix
	void updatePrices(TextUpdateOrigin whoCalled, unsigned int priceIndex);

	// Met à jour tous les éléments graphiques
	std::function<void()> updateVisualization;

	// Change l'état d'un module (arguments : numéro du module, état à affecter au module)
	std::function<void(int, const ErrModule&)> setModuleState;

	// Retourne l'état d'un module (argument : numéro du module)
	std::function<ErrModule(int)> getModuleState;

	std::function<void()> sendSequence;

	// Ouvre le panneau de gauche
	std::function<void()> openSettings;

	// Ferme le panneau de gauche
	std::function<void()> closeSettings;

	// Affiche le clavier visuel
	std::function<void(KeyboardLabel*, const std::function<bool(const String&)>&, const String&, unsigned int)> showKeyboard;

	// Ouvre le panneau de sélection de la station essence
	std::function<void()> selectGasStation;

	// Ouvre le panneau de sélection du panneau d'affichage
	std::function<void()> selectPanel;

	// Ouvre le panneau de sélection de l'UC
	std::function<void()> selectUC;

	// Ferme le panneau de sélection de l'UC
	std::function<void()> closeUCSelection;

	// Ferme tous les panneaux de sélection
	std::function<void()> closeAllSelections;

	//==============================================================================

	// Retourne le tableau de prix
	Price* getPrices() { return prices; }

	// Retourne le JSON des prix (pour être sauvegardés)
	PricesJSON* getpricesjson() { return pricesjson; }

	// Retourne le JSON des stations services avec une requete réseau
	String getGasStationsjson(bool& success) { return network.getAllGasStations(success); };

	// Retourne le JSON des panneaux avec une requete réseau
	String getPanelsjson(int index, bool& success) { return network.getPanels(index, success); }

	// Retourne le JSON des UC avec une requete réseau
	String getUCsjson(int index, bool& success) { return network.getUCs(index, success); }

	// Ne pas utiliser
	bool getBatteryAlarm();

	// Retourne la liste des caractères acceptés par l'éditeur de prix (chiffres, virgule, point)
	StringRef getDigitEditorAcceptedCharacters() { return digitEditorAcceptedCharacters; }

	// Marque le logiciel comme initialisé
	void init() { initBool = true; }

	// Marque le logiciel comme non-initialisé
	void resetInit() { initBool = false; resetAllStates(); }

	// Met tous les états
	void resetAllStates()
	{
		for (int i = 0; i < numPrices * numDigits; i++)
			setModuleState(i, ErrModule::white());
	}
	bool isInit() { return initBool; }

	// Ouvre la fenêtre d'alerte désirée
	void openAlertWindow(APPCAlertWindows::WindowType window, const String& message = "", const std::function<void(int)>&callbackfunction = std::function<void(int)>())
	{
		alertWindows.open(window, message, callbackfunction);
	}
	void closeAlertWindow() { alertWindows.close(); }

	// Crée la séquence de chiffres à afficher en fonction de quels chiffres ne sont pas à jour
	Sequence createOptimizedSequence();

private:
	//==============================================================================

	// Conctructeur 
	Core();

	unsigned int numDigits, numPrices, delay_ms, id, COM, stationID, panelID;
	bool networkInit, lineControl, resetLine, isInTransmission, playSequence, connected, inTesting, initBool, inSelection;
	std::function<void(TextUpdateOrigin, unsigned int)> pricesUpdateFunction;
	ConfigJSON* configjson;
	PricesJSON* pricesjson;
	Network network;
	Price prices[MAX_PRICES];
	std::shared_ptr<APPCLookAndFeel> lfptr;
	Sequence sequence;
	String digitEditorAcceptedCharacters, UCName;
	APPCAlertWindows alertWindows;
	JUCE_LEAK_DETECTOR(Core)
};