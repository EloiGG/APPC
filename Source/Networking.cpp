/*
  ==============================================================================

	Networking.cpp
	Created: 16 Jun 2021 10:59:25am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "Networking.h"

bool Network::err_ok(const String& output)
{
	return output.substring(0, 6) != String("Failed") &&
		output != "{\"message\":\"Username could not be found.\"}";
}

std::tuple<bool, int> Network::connected() const
{
	URL realURL(url + String("/fuel_prices"));
	StringPairArray responseHeaders;
	int statusCode = 0;
	if (auto stream = std::unique_ptr<InputStream>(realURL.createInputStream(false, nullptr, nullptr, makeHeader(),
		5000, // timeout in millisecs
		&responseHeaders, &statusCode)))
	{
		if (statusCode == 200)
			return { true, statusCode };
	}

	return { false, statusCode };
}

Network::Network() : url("https://centofuel2.centaure-systems.fr/api")
{
}

Network::Network(const String& token, const String& password) : authToken(token), authPassword(password),
url("https://centofuel2.centaure-systems.fr/api")
{
}

String Network::getFuelPrice(int timeout_ms) const
{
	return request("/fuel_prices");
}

void Network::setAuthentication(const String& token, const String& password)
{
	authToken = token;
	authPassword = password;
}

String Network::getOilCompany(int id) const
{
	return request(String("/oil_companies/") + String(id));
}

String Network::getMotorwayCompany(int id) const
{
	return request(String("/motorway_companies/") + String(id));
}

String Network::getAllGasStations(int timeout_ms) const
{
	return request("/gas_stations");
}

String Network::getPanels(int id) const
{
	return request(String("/maintenance/gas_stations/") + String(id));
}

String Network::getUCs(int id) const
{
	return request(String("/maintenance/panels/") + String(id));
}

String Network::request(const String& requestString, int timeout_ms) const
{
	URL realURL(url + requestString);
	StringPairArray responseHeaders;
	int statusCode = 0;
	jassert(authPassword != ""); // Il n'y a pas eu d'authentification. utiliser setAuthentication
	if (auto stream = std::unique_ptr<InputStream>(realURL.createInputStream(false, nullptr, nullptr, makeHeader(),
		timeout_ms, // timeout in millisecs
		&responseHeaders, &statusCode)))
	{
		return (stream->readEntireStreamAsString());
	}

	if (statusCode != 0)
		return "Failed to connect, status code = " + String(statusCode);

	return "Failed to connect!";
}

String Network::makeHeader() const
{
	return authToken + String(": ") + authPassword;
}
