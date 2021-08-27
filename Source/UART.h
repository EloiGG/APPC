/*
  ==============================================================================

	UART.h
	Created: 16 Jun 2021 10:59:35am
	Author:  Eloi GUIHARD-GOUJON

	Contient la classe UART qui permet la transmission de données via le port série.
	Compatible Windows uniquement.

  ==============================================================================
*/

#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <string>

/// <summary>
/// Cette classe permet la transmission des données via le port série.
/// Pour transmettre des données, d'abord ouvrir le port avec la méthode open(),
/// ensuite ajouter des octets avec la méthode addByte() puis envoyer avec la méthode send().
/// Fermer le port avec close()
/// Pour en recevoir, utiliser la méthode read() 
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

	// Lit le dernier octet reçu et le place dans data. Retourne vrai si l'oparation a été un succès
	bool read(unsigned char* data);

	// Ajoute un octet au buffer. Rien n'est envoyé tant que send() n'a pas été appelé.
	void addByte(unsigned char Byte);

	// Ne pas utiliser
	void addBytes(const char* s);

	// Envoie tous les octets du buffer à travers le port série. 
	bool send();

	// Ferme le port
	bool close();

	// Retourne vrai si le port série est accessible
	static bool checkCOMPort(int comPort);

//==============================================================================
private:
	// Paramètres du port COM	//
	HANDLE hSerial;				//
	DCB dcbSerialParams;		//
	COMMTIMEOUTS timeouts;		//

	// Buffer interne				//
	unsigned char bytes[MAX_BYTES];	// Buffer
	size_t numBytesInBuffer;		// Taille du buffer
};