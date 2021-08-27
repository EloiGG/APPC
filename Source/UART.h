/*
  ==============================================================================

	UART.h
	Created: 16 Jun 2021 10:59:35am
	Author:  Eloi GUIHARD-GOUJON

	Contient la classe UART qui permet la transmission de donn�es via le port s�rie.
	Compatible Windows uniquement.

  ==============================================================================
*/

#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <string>

/// <summary>
/// Cette classe permet la transmission des donn�es via le port s�rie.
/// Pour transmettre des donn�es, d'abord ouvrir le port avec la m�thode open(),
/// ensuite ajouter des octets avec la m�thode addByte() puis envoyer avec la m�thode send().
/// Fermer le port avec close()
/// Pour en recevoir, utiliser la m�thode read() 
/// </summary>
class UART
{
//==============================================================================
public:
	static constexpr size_t MAX_BYTES = 16;

	enum class Parity { noParity = NOPARITY, oddParity = ODDPARITY, evenParity = EVENPARITY, markParity = MARKPARITY, spaceParity = SPACEPARITY };
	enum class StopBit { oneStopBit = ONESTOPBIT, one5StopBits = ONE5STOPBITS, twoStopBits = TWOSTOPBITS };
	enum class BaudRate {
		baud_110 = 110,
		baud_300 = 300,
		baud_600 = 600,
		baud_1200 = 1200,
		baud_2400 = 2400,
		baud_4800 = 4800,
		baud_9600 = 9600,
		baud_14400 = 14400,
		baud_19200 = 19200,
		baud_38400 = 38400,
		baud_56000 = 56000,
		baud_57600 = 57600,
		baud_115200 = 115200,
		baud_128000 = 128000,
		baud_256000 = 256000
	};

	UART();

	// Ouvre le port
	bool open(int baudRate, unsigned int byteSize, StopBit stopbit, Parity parity, int COM);

	// Lit le dernier octet re�u et le place dans data. Retourne vrai si l'oparation a �t� un succ�s
	bool read(unsigned char* data);

	// Ajoute un octet au buffer. Rien n'est envoy� tant que send() n'a pas �t� appel�.
	void addByte(unsigned char Byte);

	// Ne pas utiliser
	void addBytes(const char* s);

	// Envoie tous les octets du buffer � travers le port s�rie. 
	bool send();

	// Ferme le port
	bool close();

	// Retourne vrai si le port s�rie est accessible
	static bool checkCOMPort(int comPort);

//==============================================================================
private:
	// Param�tres du port COM	//
	HANDLE hSerial;				//
	DCB dcbSerialParams;		//
	COMMTIMEOUTS timeouts;		//

	// Buffer interne				//
	unsigned char bytes[MAX_BYTES];	// Buffer
	size_t numBytesInBuffer;		// Taille du buffer
};