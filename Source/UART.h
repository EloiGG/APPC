/*
  ==============================================================================

    UART.h
    Created: 16 Jun 2021 10:59:35am
    Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/
#pragma once

#include <windows.h>
#include <string>

class UART
{
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
	bool open(int baudRate, unsigned int byteSize, StopBit stopbit, Parity parity, int COM);
	bool read(unsigned char* data);
	void addByte(unsigned char Byte);
	void addBytes(const char* s);
	bool send();
	bool close();
private:
	HANDLE hSerial;
	DCB dcbSerialParams;
	COMMTIMEOUTS timeouts;
	unsigned char bytes[MAX_BYTES];
	size_t numBytesInBuffer;
};