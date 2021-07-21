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
#include "Networking.h"

/// <summary>
/// Contient toutes les informations autour d'un prix. Informations récupérables via les accesseurs
/// correspondants
/// </summary>
class PriceJSON
{
public:
	PriceJSON(DynamicObject* d) : dynObj(d) {}

	void setPrice(const String& newPrice);

	String getPrice() const { return dynObj->getProperty("price").toString(); }
	int getId() const { return dynObj->getProperty("id"); }
	String getGasStation() const { return dynObj->getProperty("GasStation"); }
	String getFuel() const { return dynObj->getProperty("Fuel"); }
	String getCreated() const { return dynObj->getProperty("created"); }
	String getUpdated() const { return dynObj->getProperty("updated"); }
	int getPosition() const { return dynObj->getProperty("position"); }
	bool getEmptied() const { return dynObj->getProperty("emptied"); }
	String getModifyAt() const { return dynObj->getProperty("modifyAt"); } // TYPE A VERIFIER
	bool getModifyDone() const { return dynObj->getProperty("modifyDone"); }
	bool getStatus() const { return dynObj->getProperty("status"); }
	String getOldPrice() const { return dynObj->getProperty("oldPrice"); }

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

	void setFile(const File& json) { parsedJSON = JSON::parse(json); }
	String getAuthPassword() { return parsedJSON.getProperty("api_key", "error"); }
	int getID() { return (int)parsedJSON.getProperty("id", -1); }
	String getBaseAPI() { return parsedJSON.getProperty("base_api", "error"); }
	int getLineControl() { return (bool)parsedJSON.getProperty("line_control", -1); }
	int getResetLine() { return (bool)parsedJSON.getProperty("reset_line", -1); }
	String getAPIKey() { return parsedJSON.getProperty("api_key", "error"); }
	int getDelay() { return parsedJSON.getProperty("delay", -1); }
	int getNumLines() { return parsedJSON.getProperty("number_of_lines", -1); }
	int getNumColumns() { return parsedJSON.getProperty("number_of_columns", -1); }
	int getCOMPort() { return parsedJSON.getProperty("COM_port", -1); }

	String makeConfigJSON(int id, const String& base_api, const String& api_key,
		bool line_control, bool reset_line, int delay, int numColumns, int numLines, int COMPort);
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

class GasStationJSON
{
public:
	GasStationJSON(DynamicObject* d) : dynObj(d) {}
	int getId() const { return dynObj->getProperty("id"); }
	String getName()const { return dynObj->getProperty("name"); }
	String getHighway()const { return dynObj->getProperty("highway"); }
	String getDirection()const { return dynObj->getProperty("direction"); }
	String getOilCompany()const { return dynObj->getProperty("OilCompany"); }
	void setOilCompany(const String& newOilCompany) { dynObj->setProperty("OilCompany", newOilCompany); }
	String getMotorwayCompany()const { return dynObj->getProperty("MotorwayCompany"); }
	void setMotorwayCompany(const String& newMotorwayCompany) { dynObj->setProperty("MotorwayCompany", newMotorwayCompany); }
private:
	DynamicObject* dynObj;
};

class GasStationsJSON
{
public:
	GasStationsJSON(const String& json) : parsedJSON(JSON::parse(json)) {}
	void updatePropreties(const Network& net);
	GasStationJSON operator[](size_t index) const;
	size_t getNumGasStations() const;
private:
	var parsedJSON;
};

class PanelJSON
{
public:
	PanelJSON(){}
	PanelJSON(const String& json) : parsedJSON(JSON::parse(json)), var((*parsedJSON.getArray())[0]) {}
	
	int getId() { return var.getProperty("id", "error"); }
	String getName() { return var.getProperty("name", "error"); }
	String getType() { return var.getProperty("type", "error"); }
	int getBox() {return var.getProperty("box", "error");}
	size_t getNumPanels() const { return parsedJSON.getArray()->size(); }
private:
	var parsedJSON;
	var var;
};

