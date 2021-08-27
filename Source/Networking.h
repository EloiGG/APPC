/*
  ==============================================================================

    Networking.h
    Created: 16 Jun 2021 10:59:25am
    Author:  Eloi GUIHARD-GOUJON

    Contient la classe Network qui g�re tous les acc�s � CentoFuel.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <tuple>
#include "Log.h"

/// <summary>
/// La classe Network g�re toutes les requ�tes � CentoFuel. Elle garde en m�moire 
/// le mot de passe. Une fois cr��e avec le mot de passe en argument d'initialisation,
/// la classe peut retourner le r�sultat de n'importe quelle requ�te � CentoFuel.
/// </summary>
class Network 
{
//==============================================================================
public:
    // Fonction ancienne, ne pas utiliser
    static bool err_ok(const String& output);

    // Retourne un tuple : un bool�en qui est vrai si la connexion est �tablie et un int pour le code retour
    std::tuple<bool, int> connected() const;

//==============================================================================
public:
    Network();
    Network(const String& token, const String& password, const String& baseAPI);
    
    // Set l'URL de l'API CentoFuel
    void setURL(const String& newURL) { url = newURL; }
    String getURL() { return url; }

    // Set le mot de passe
    void setAuthentication(const String& token, const String& password);

    // Retourne le r�sultat de la requ�te "fuel_prices"
    String getFuelPrice(bool& success, int timeout_ms = 10000) const;

    // Retourne le mot de passe 
    String getPassword() const { return authPassword; }

    // Retourne le r�sultat de la requ�te "oil_companies"
    String getOilCompany(int id, bool& success) const;

    // Retourne le r�sultat de la requ�te "motorway_companies"
    String getMotorwayCompany(int id, bool& success) const;

    // Retourne le r�sultat de la requ�te "gas_stations"
    String getAllGasStations(bool& success, int timeout_ms = 10000) const;

    // Retourne le r�sultat de la requ�te "maintenance/gas_stations"
    String getPanels(int id, bool& success) const;

    // Retourne le r�sultat de la requ�te "maintenance/panels"
    String getUCs(int id, bool& success) const;

    // Retourne le r�sultat de la requ�te pass�e en argument
    String request(const String& requestString, bool& success, int timeout_ms = 10000) const;

//==============================================================================
private:
    String makeHeader() const;
    String authToken, authPassword;
    String url;
};
