/*
  ==============================================================================

	COMThread.cpp
	Created: 20 Jul 2021 8:10:41am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "COMThread.h"

void COMThread::run()
{
	int i;
	for (i = 1; i <= 30; i++)
	{
		setProgression(float(i) / 30);
		if (uart.open(1170, 8, UART::StopBit::oneStopBit, UART::Parity::noParity, i)) {
			if (verifyCOMPort(i)) {
				setProgression(1);
				DBG("TROUVE : " << i);
				uart.close();
				wait(100);
				Core::get().setCOMPort(i);
				goto fin;
			}
			uart.close();
			wait(100);
			goto fin;
		}
		wait(333);
		if (threadShouldExit() || exitAsked)
			break;
	}
fin:
	MessageManagerLock mml;
	if (mml.lockWasGained()) {
		Core::get().setInTesting(false);
		Core::get().updateVisualization();
	}
	exitAsked = false;
	return;
}

bool COMThread::verifyCOMPort(unsigned int portNum)
{
	int numModulesToTest = 5;
	for (int i = 0; i < numModulesToTest; i++) {
		DBG("test module " << i);
		uart.addByte(0x02);
		uart.addByte(0x49);
		uart.addByte(0x30 + i);
		uart.addByte(0x38);
		uart.addByte(0x03);
		uart.send();
		wait(100);
		if (getModuleResponse(i))
			return true;
	}
	return false;
}

bool COMThread::getModuleResponse(int digitNumber)
{
	int timeout_ms = 100;
	s.reset();
	unsigned char c = 0;
	while (c != 0x02) {
		wait(30);
		if (threadShouldExit() || exitAsked)
			return false;
		uart.read(&c);		//...0x02
		if (s.elapsed_time_ms() > timeout_ms)
			return false;
	}
	uart.read(&c);			//0x52
	uart.read(&c);			//<adresse>
	if (c == digitNumber + 0x30)
		return true;
}