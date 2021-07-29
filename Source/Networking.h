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
    Network(const String& token, const String& password, const String& baseAPI);
    
    void setURL(const String& newURL) { url = newURL; }
    String getFuelPrice(bool& success, int timeout_ms = 10000) const;
    void setAuthentication(const String& token, const String& password);
    String getPassword() const { return authPassword; }
    String getOilCompany(int id, bool& success) const;
    String getMotorwayCompany(int id, bool& success) const;
    String getAllGasStations(bool& success, int timeout_ms = 10000) const;
    String getPanels(int id, bool& success) const;
    String getUCs(int id, bool& success) const;
    String request(const String& requestString, bool& success, int timeout_ms = 10000) const;
    String getURL() { return url; }
private:
    String makeHeader() const;
    String authToken, authPassword;
    String url;
};
