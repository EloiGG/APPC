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

Network::Network() : url("https://centofuel2.centaure-systems.fr/api")
{
}

Network::Network(const String& token, const String& password) : authToken(token), authPassword(password),
url("https://centofuel2.centaure-systems.fr/api")
{
}

String Network::getFuelPrice(int timeout_ms)
{
	url += String("/fuel_prices");
	URL realURL(url);
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

String Network::makeHeader()
{
	return authToken + String(": ") + authPassword;
}
