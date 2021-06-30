/*
  ==============================================================================

	JSON.h
	Created: 16 Jun 2021 11:29:44am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Log.h"

/// <summary>
/// Contient toutes les informations autour d'un prix. Informations récupérables via les accesseurs
/// correspondants
/// </summary>
class PriceJSON
{
public:
	PriceJSON(DynamicObject* d);

	void setPrice(const String& newPrice);
	String getPrice() const;
	int getId() const; 
	String getGasStation() const;
	String getFuel() const;
	String getCreated() const;
	String getUpdated() const;
	int getPosition() const;
	bool getEmptied() const;
	String getModifyAt() const; // TYPE A VERIFIER
	bool getModifyDone() const;
	bool getStatus() const;
	String getOldPrice() const;

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
	int getNumLines();
	int getNumColumns();

	String makeConfigJSON(int id, const String& base_api, const String& api_key, bool line_control, bool reset_line, int delay, int numColumns, int numLines);
	String toString();

private:
	var parsedJSON;
};

class SequenceJSON
{
public:
	SequenceJSON(const File& json = File());
	String getPrice(int sequenceStep, int price);
	int getSequenceSize();
	int getNumPrices();
private:
	int sequenceSize;
	int numPrices;
	var parsedJSON;
};

class PriceSave
{
public:
	PriceSave(const File& file = File());
	String getPrice(int index);
	static String makePriceSave(const String* prices, int numPrices);
	int getNumPrices();

private:
	var parsedPriceSave;
};