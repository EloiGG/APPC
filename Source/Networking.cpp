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
		3000, // timeout in millisecs
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

Network::Network(const String& token, const String& password, const String& baseAPI) : authToken(token), authPassword(password),
url(baseAPI)
{
}

String Network::getFuelPrice(bool& success, int timeout_ms) const
{
	return request("/fuel_prices", success);
}

void Network::setAuthentication(const String& token, const String& password)
{
	authToken = token;
	authPassword = password;
}

String Network::getOilCompany(int id, bool& success) const
{
	return request(String("/oil_companies/") + String(id), success);
}

String Network::getMotorwayCompany(int id, bool& success) const
{
	return request(String("/motorway_companies/") + String(id), success);
}

String Network::getAllGasStations(bool& success, int timeout_ms) const
{
	return request("/gas_stations", success);
}

String Network::getPanels(int id, bool& success) const
{
	return request(String("/maintenance/gas_stations/") + String(id), success);
}

String Network::getUCs(int id, bool& success) const
{
	return request(String("/maintenance/panels/") + String(id), success);
}

String Network::request(const String& requestString, bool& success, int timeout_ms) const
{
	URL realURL(url + requestString);
	StringPairArray responseHeaders;
	int statusCode = 0;
	if (authPassword == "") {
		success = false;
		return "No password";
	}// Il n'y a pas eu d'authentification. utiliser setAuthentication
	if (auto stream = std::unique_ptr<InputStream>(realURL.createInputStream(false, nullptr, nullptr, makeHeader(),
		timeout_ms, // timeout in millisecs
		&responseHeaders, &statusCode)))
	{
		if (statusCode != 200) {
			success = false;
			return "Failed to connect, status code = " + String(statusCode);
		}
		success = true;
		return (stream->readEntireStreamAsString());
	}
	success = false;
	if (statusCode != 0)
		return "Failed to connect, status code = " + String(statusCode);

	return "Failed to connect!";
}

String Network::makeHeader() const
{
	return authToken + String(": ") + authPassword;
}
