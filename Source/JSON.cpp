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
	Log::writeLn("Lecture du fichier JSON ");
	Log::writeLn(json.getFullPathName());
	Log::ln();

	parsedJSON = JSON::parse(json);

	if (parsedJSON == var())
		Log::writeLn("Impossible de lire le fichier");
	else
		Log::writeLn("Lecture reussie");
	Log::ln();
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

void PriceJSON::setPrice(const String& newPrice)
{
	dynObj->setProperty("oldPrice", getPrice());
	dynObj->setProperty("price", newPrice.getFloatValue());
}

bool ConfigJSON::err_ok(const String& output)
{
	return output != "error";
}

ConfigJSON::ConfigJSON(const File& json)
{
	Log::writeLn("Lecture du fichier JSON ");
	Log::writeLn(json.getFullPathName());

	parsedJSON = JSON::parse(json);

	if (parsedJSON == var())
		Log::writeLn("Impossible de lire le fichier");
	else
		Log::writeLn("Lecture reussie");
	Log::ln();
	Log::update();
}

String ConfigJSON::makeConfigJSON(const String& base_api, const String& api_key, bool line_control, int numColumns, int numLines, int COMPort, const String& startingDigit)
{
	DynamicObject* obj = new DynamicObject();
	obj->setProperty("base_api", base_api);
	obj->setProperty("api_key", api_key);
	obj->setProperty("line_control", line_control);
	obj->setProperty("number_of_columns", numColumns);
	obj->setProperty("number_of_lines", numLines);
	obj->setProperty("COM_port", COMPort);
	obj->setProperty("starting_character", startingDigit);

	var json(obj);

	return JSON::toString(json);
}

String ConfigJSON::toString()
{
	return makeConfigJSON(getBaseAPI(), getBaseAPI(), getLineControl(), getNumColumns(), getNumLines(), getCOMPort(), getStartingCharacter());
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

void GasStationsJSON::updatePropreties(const Network& net)
{
	auto v = *parsedJSON.getArray();
	for (int i = 0; i < getNumGasStations(); i++) {
		auto w = v[i].getDynamicObject();
		bool success;
		int oilCompanyNumber = w->getProperty("OilCompany").toString().fromLastOccurrenceOf("/", false, true).getIntValue();
		String oilCompanyName((JSON::parse(net.getOilCompany(oilCompanyNumber, success)).getProperty("name", "error")).toString());
		w->setProperty("OilCompany", oilCompanyName);

		int motorwayCompanyNumber = w->getProperty("MotorwayCompany").toString().fromLastOccurrenceOf("/", false, true).getIntValue();
		String motorwayCompanyName((JSON::parse(net.getMotorwayCompany(motorwayCompanyNumber, success)).getProperty("name", "error")).toString());
		w->setProperty("MotorwayCompany", motorwayCompanyName);
	}
}

GasStationJSON GasStationsJSON::operator[](size_t index) const
{
	auto v = *parsedJSON.getArray();
	auto w = v[index].getDynamicObject();
	return GasStationJSON(w);
}

size_t GasStationsJSON::getNumGasStations() const
{
	return parsedJSON.getArray()->size();
}

UCLinePrice UCJSON::getUCLinePrice(unsigned int index)const
{
	auto v = parsedJSON.getDynamicObject();
	auto& w = *v->getProperty("linesPrices").getArray();
	auto x = w[index].getDynamicObject();
	return UCLinePrice(x);
}

StationLinePrice UCJSON::getStationLinePrice(unsigned int index)const
{
	auto v = parsedJSON.getDynamicObject();
	auto& w = *v->getProperty("linesPrices").getArray();
	auto x = w[index].getDynamicObject();
	auto y = x->getProperty("fuelPrice").getDynamicObject();
	return StationLinePrice(y);
}
