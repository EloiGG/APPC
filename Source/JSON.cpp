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

String ConfigJSON::getAuthToken()
{
    return parsedJSON.getProperty("authenticationToken", "error");
}

String ConfigJSON::getAuthPassword()
{
    return parsedJSON.getProperty("authenticationPassword", "error");
}