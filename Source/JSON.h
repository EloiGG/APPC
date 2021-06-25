/*
  ==============================================================================

	JSON.h
	Created: 16 Jun 2021 11:29:44am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/// <summary>
/// Contient toutes les informations autour d'un prix. Informations récupérables via les accesseurs
/// correspondants
/// </summary>
class PriceJSON
{
public:
	PriceJSON(DynamicObject* d);

	String getPrice();
	int getId(); 
	String getGasStation();
	String getFuel();
	String getCreated();
	String getUpdated();
	int getPosition();
	bool getEmptied();
	String getModifyAt(); // TYPE A VERIFIER
	bool getModifyDone();
	bool getStatus();
	String getOldPrice();

private:
	DynamicObject* dynObj;
};

/// <summary>
/// Cette classe contient tout le texte du fichier JSON récupéré par la requête fuel_prices
/// Ce fichier peut contenir plusieurs prix différents. Ils sont accessibles individuellement
/// sous la forme PriceJSON via l'opérateur [].
/// </summary>
class PricesJSON
{
public:
	PricesJSON(const String& json);
	PricesJSON(const File& json);

	size_t getNumPrices();
	float getPrice();

	PriceJSON operator[](int index);
private:
	var parsedJSON;
};

/// <summary>
/// Cette classe contient les informations du fichier JSON de configuration du logiciel.
/// </summary>
class ConfigJSON
{
public:
	static bool err_ok(const String& output);

public:
	ConfigJSON(const File& json = File());
	//~ConfigJSON();
	void setFile(const File& json);
	String getAuthPassword();
	int getID();
	String getBaseAPI();
	int getLineControl();
	int getResetLine();
	String getAPIKey();
	int getDelay();

	String makeConfigJSON(int id, const String& base_api, const String& api_key, bool line_control, bool reset_line, int delay);
	String toString();

private:
	var parsedJSON;
};