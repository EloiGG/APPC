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
	Log::write(L"Test de communication avec CentoFuel : ");
	URL realURL(url + String("/fuel_prices"));
	StringPairArray responseHeaders;
	int statusCode = 0;
	if (auto stream = std::unique_ptr<InputStream>(realURL.createInputStream(false, nullptr, nullptr, makeHeader(),
		5000, // timeout in millisecs
		&responseHeaders, &statusCode)))
	{
		Log::write(L"test r�ussi !\n");
		if (statusCode == 200)
			return { true, statusCode };
	}

	Log::write(L"�chec de la connexion au r�seau !\n");
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
	URL realURL(url + String("/fuel_prices"));
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



void Network::setAuthentication(const String& token, const String& password)
{
	authToken = token;
	authPassword = password;
}

String Network::makeHeader() const
{
	return authToken + String(": ") + authPassword;
}
