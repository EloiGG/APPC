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
    parsedJSON = JSON::parse(json);
}

size_t PricesJSON::getNumPrices()
{
    return parsedJSON.getArray()->size();
}

float PricesJSON::getPrice()
{
    auto v = *parsedJSON.getArray();
    auto w = v[1].getDynamicObject();
    auto x = w->getProperty("id");
    return 0.0f;
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

String PriceJSON::getPrice()
{
    return dynObj->getProperty("price").toString();
}

int PriceJSON::getId()
{
    return dynObj->getProperty("id");
}

String PriceJSON::getGasStation()
{
    return dynObj->getProperty("GasStation");
}

String PriceJSON::getFuel()
{
    return dynObj->getProperty("Fuel");
}

String PriceJSON::getCreated()
{
    return dynObj->getProperty("created");
}

String PriceJSON::getUpdated()
{
    return dynObj->getProperty("updated");
}

int PriceJSON::getPosition()
{
    return dynObj->getProperty("position");
}

bool PriceJSON::getEmptied()
{
    return dynObj->getProperty("emptied");
}

String PriceJSON::getModifyAt()
{
    return dynObj->getProperty("modifyAt");
}

bool PriceJSON::getModifyDone()
{
    return dynObj->getProperty("modifyDone");
}

bool PriceJSON::getStatus()
{
    return dynObj->getProperty("status");
}

String PriceJSON::getOldPrice()
{
    return dynObj->getProperty("oldPrice");
}

bool ConfigJSON::err_ok(const String& output)
{
    return output != "error";
}

ConfigJSON::ConfigJSON(const File& json)
{
    parsedJSON = JSON::parse(json);
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
    return (bool)parsedJSON.getProperty("line_control",-1);
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

String ConfigJSON::makeConfigJSON(int id, const String& base_api, const String& api_key, bool line_control, bool reset_line, int delay)
{
    DynamicObject* obj = new DynamicObject();
    obj->setProperty("id", id);
    obj->setProperty("base_api", base_api);
    obj->setProperty("api_key", api_key);
    obj->setProperty("line_control", line_control);
    obj->setProperty("reset_line", reset_line);

    var json(obj); // store the outer object in a var [we could have done this earlier]

    return JSON::toString(json);
}

String ConfigJSON::toString()
{
    return makeConfigJSON(getID(), getBaseAPI(), getBaseAPI(), getLineControl(), getResetLine(), getDelay());
}
