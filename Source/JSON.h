/*
  ==============================================================================

	JSON.h
	Created: 16 Jun 2021 11:29:44am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "rapidjson/rapidjson.h"

/// <summary>
/// Contient toutes les informations autour d'un prix. Informations r�cup�rables via les accesseurs
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
/// Cette classe contient tout le texte du fichier JSON r�cup�r� par la requ�te fuel_prices
/// Ce fichier peut contenir plusieurs prix diff�rents. Ils sont accessibles individuellement
/// sous la forme PriceJSON via l'op�rateur [].
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
