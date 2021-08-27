/*
  ==============================================================================

	UART.cpp
	Created: 16 Jun 2021 10:59:35am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "UART.h"

UART::UART() : dcbSerialParams{ 0 }, timeouts{ 0 }, numBytesInBuffer(0)
{
}

bool UART::open(int baudRate, unsigned int byteSize, StopBit stopbit, Parity parity, int COM)
{
	std::string s = std::string("\\\\.\\COM") + std::to_string(COM);
	hSerial = CreateFile(s.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hSerial, &dcbSerialParams) == 0)
	{
		CloseHandle(hSerial);
		return false;
	}

	dcbSerialParams.BaudRate = (int)baudRate;
	dcbSerialParams.ByteSize = byteSize;
	dcbSerialParams.StopBits = (int)stopbit;
	dcbSerialParams.Parity = (int)parity;
	if (SetCommState(hSerial, &dcbSerialParams) == 0)
	{
		CloseHandle(hSerial);
		return false;
	}

	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hSerial, &timeouts) == 0)
	{
		CloseHandle(hSerial);
		return false;
	}

	return true;
}

bool UART::read(unsigned char* data)
{
	return ReadFile(hSerial, data, 1, NULL, NULL);
}

void UART::addByte(unsigned char Byte)
{
	bytes[numBytesInBuffer] = Byte;
	numBytesInBuffer++;
}

void UART::addBytes(const char* s)
{ // cette fonction ne doit pas être utilisée
	for (int i = 0; i < 15; i += 3) {
		unsigned char a = s[i] > '9' ? s[i] - 87 : s[i] - 48;
		unsigned char b = s[i + 1] > '9' ? s[i + 1] - 87 : s[i + 1] - 48;
		unsigned char c = 16 * a + b;
		addByte(c);
	}
}

bool UART::send()
{
	DWORD bytes_written, total_bytes_written = 0;
	if (!WriteFile(hSerial, bytes, numBytesInBuffer, &bytes_written, NULL))
	{
		CloseHandle(hSerial);
		numBytesInBuffer = 0;
		return false;
	}
	numBytesInBuffer = 0;
	return true;

}

bool UART::close()
{
	if (CloseHandle(hSerial) == 0)
	{
		return false;
	}
	return true;
}

bool UART::checkCOMPort(int comPort)
{
	UART uart;
	if (uart.open(1170, 8, UART::StopBit::oneStopBit, UART::Parity::noParity, comPort)) {
		uart.close();
		return true;
	}
	return false;
}
