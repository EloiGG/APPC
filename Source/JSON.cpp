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

String ConfigJSON::makeConfigJSON(int id, const String& base_api, const String& api_key, bool line_control, bool reset_line, int delay, int numColumns, int numLines, int COMPort)
{
	DynamicObject* obj = new DynamicObject();
	obj->setProperty("id", id);
	obj->setProperty("base_api", base_api);
	obj->setProperty("api_key", api_key);
	obj->setProperty("line_control", line_control);
	obj->setProperty("reset_line", reset_line);
	obj->setProperty("number_of_columns", numColumns);
	obj->setProperty("number_of_lines", numLines);
	obj->setProperty("COM_port", COMPort);

	var json(obj);

	return JSON::toString(json);
}

String ConfigJSON::toString()
{
	return makeConfigJSON(getID(), getBaseAPI(), getBaseAPI(), getLineControl(), getResetLine(), getDelay(), getNumColumns(), getNumLines(), getCOMPort());
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
	//for (int i = 0; i < getNumGasStations(); i++)
	//	operator[](i).setOilCompany(net.request(operator[](i).getOilCompany().substring(4)));
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
