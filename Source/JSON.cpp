/*
  ==============================================================================

	JSON.cpp
	Created: 16 Jun 2021 11:29:44am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "JSON.h"

PricesJSON::PricesJSON(const String& json)
{
	parsedJSON = JSON::parse(json);
}

PricesJSON::PricesJSON(const File& json)
{
	Log::write("\nLecture du fichier JSON ");
	Log::write(json.getFullPathName());
	Log::ln();

	parsedJSON = JSON::parse(json);

	if (parsedJSON == var())
		Log::write("Impossible de lire le fichier");
	else
		Log::write(L"Lecture réussie");
	Log::ln(1, 2);
	Log::update();
}


size_t PricesJSON::getNumPrices()
{
	return parsedJSON.getArray()->size();
}


PriceJSON PricesJSON::operator[](int index)
{
	auto v = *parsedJSON.getArray();
	auto w = v[index].getDynamicObject();
	return PriceJSON(w);
}


PriceJSON::PriceJSON(DynamicObject* d) : dynObj(d)
{
}

String PriceJSON::getPrice() const
{
	return dynObj->getProperty("price").toString();
}

void PriceJSON::setPrice(const String& newPrice)
{
	dynObj->setProperty("oldPrice", getPrice());
	dynObj->setProperty("price", newPrice.getFloatValue());
}

int PriceJSON::getId() const
{
	return dynObj->getProperty("id");
}

String PriceJSON::getGasStation() const
{
	return dynObj->getProperty("GasStation");
}

String PriceJSON::getFuel() const
{
	return dynObj->getProperty("Fuel");
}

String PriceJSON::getCreated() const
{
	return dynObj->getProperty("created");
}

String PriceJSON::getUpdated() const
{
	return dynObj->getProperty("updated");
}

int PriceJSON::getPosition() const
{
	return dynObj->getProperty("position");
}

bool PriceJSON::getEmptied() const
{
	return dynObj->getProperty("emptied");
}

String PriceJSON::getModifyAt() const
{
	return dynObj->getProperty("modifyAt");
}

bool PriceJSON::getModifyDone() const
{
	return dynObj->getProperty("modifyDone");
}

bool PriceJSON::getStatus() const
{
	return dynObj->getProperty("status");
}

String PriceJSON::getOldPrice() const
{
	return dynObj->getProperty("oldPrice");
}

bool ConfigJSON::err_ok(const String& output)
{
	return output != "error";
}

ConfigJSON::ConfigJSON(const File& json)
{
	Log::write("\nLecture du fichier JSON ");
	Log::write(json.getFullPathName());
	Log::ln();

	parsedJSON = JSON::parse(json);

	if (parsedJSON == var())
		Log::write("Impossible de lire le fichier");
	else
		Log::write(L"Lecture réussie");
	Log::ln(1, 2);
	Log::update();
}

void ConfigJSON::setFile(const File& json)
{
	parsedJSON = JSON::parse(json);
}

String ConfigJSON::getAuthPassword()
{
	return parsedJSON.getProperty("api_key", "error");
}

int ConfigJSON::getID()
{
	return (int)parsedJSON.getProperty("id", -1);
}

String ConfigJSON::getBaseAPI()
{
	return parsedJSON.getProperty("base_api", "error");
}

int ConfigJSON::getLineControl()
{
	return (bool)parsedJSON.getProperty("line_control", -1);
}

int ConfigJSON::getResetLine()
{
	return (bool)parsedJSON.getProperty("reset_line", -1);
}

String ConfigJSON::getAPIKey()
{
	return parsedJSON.getProperty("api_key", "error");
}

int ConfigJSON::getDelay()
{
	return parsedJSON.getProperty("delay", -1);
}

int ConfigJSON::getNumLines()
{
	return parsedJSON.getProperty("number_of_lines", -1);
}

int ConfigJSON::getNumColumns()
{
	return parsedJSON.getProperty("number_of_columns", -1);
}

String ConfigJSON::makeConfigJSON(int id, const String& base_api, const String& api_key, bool line_control, bool reset_line, int delay, int numColumns, int numLines)
{
	DynamicObject* obj = new DynamicObject();
	obj->setProperty("id", id);
	obj->setProperty("base_api", base_api);
	obj->setProperty("api_key", api_key);
	obj->setProperty("line_control", line_control);
	obj->setProperty("reset_line", reset_line);
	obj->setProperty("number_of_columns", numColumns);
	obj->setProperty("number_of_lines", numLines);

	var json(obj);

	return JSON::toString(json);
}

String ConfigJSON::toString()
{
	return makeConfigJSON(getID(), getBaseAPI(), getBaseAPI(), getLineControl(), getResetLine(), getDelay(), getNumColumns(), getNumLines());
}

SequenceJSON::SequenceJSON(const File& json)
{
	parsedJSON = JSON::parse(json);
	sequenceSize = parsedJSON.getArray()->size();
	auto a = parsedJSON.getArray();
	numPrices = a->operator[](0).getProperty("prices", "error").getArray()->size();
	String b = a->operator[](0).getProperty("prices", "error").getArray()->operator[](0);
}

String SequenceJSON::getPrice(int sequenceStep, int price)
{
	auto a = parsedJSON.getArray();
	return a->operator[](sequenceStep).getProperty("prices", "error").getArray()->operator[](price);
}

int SequenceJSON::getSequenceSize()
{
	return sequenceSize;
}

int SequenceJSON::getNumPrices()
{
	return numPrices;
}

PriceSave::PriceSave(const File& file)
{
	parsedPriceSave = JSON::parse(file);
}

String PriceSave::getPrice(int index)
{
	auto& v = *parsedPriceSave.getProperty("prices", "error").getArray();
	return v[index];
}

String PriceSave::makePriceSave(const String* prices, int numPrices)
{
	DynamicObject* obj = new DynamicObject();
	var p;
	for (int i = 0; i < numPrices; i++)
		p.append(prices[i]);
	obj->setProperty("prices", p);

	var json(obj);

	return JSON::toString(json);
}

int PriceSave::getNumPrices()
{
	return parsedPriceSave.getProperty("prices", "error").getArray()->size();
}
