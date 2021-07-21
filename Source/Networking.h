/*
  ==============================================================================

    Networking.h
    Created: 16 Jun 2021 10:59:25am
    Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include <tuple>
#include "Log.h"

class Network 
{
public:
    static bool err_ok(const String& output);
    std::tuple<bool, int> connected() const;

public:

    Network();
    Network(const String& token, const String& password);
    String getFuelPrice(int timeout_ms = 10000) const;
    void setAuthentication(const String& token, const String& password);
    String getPassword() const { return authPassword; }
    String getOilCompany(int id) const;
    String getAllGasStations(int timeout_ms = 10000) const;
    String getPanels(int id) const;
    String request(const String& requestString, int timeout_ms = 10000) const;

private:
    String makeHeader() const;
    String authToken, authPassword;
    String url;
};
