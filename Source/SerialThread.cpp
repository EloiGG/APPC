/*
  ==============================================================================

	Serial.cpp
	Created: 23 Jun 2021 10:29:11am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "SerialThread.h"

SerialThread::SerialThread() : Thread("SerialThread"), progression(0.0f)
{
}

SerialThread::SerialThread(const Sequence& s) : Thread("SerialThread"), sequence(s), progression(0)
{
	setPriority(9);
}

void SerialThread::setSequence(const Sequence& s)
{
	sequence = s;
	progression = 0.0f;
}

float SerialThread::getProgression() const
{
	return progression;
}

void SerialThread::run()
{
	auto check = [this]() 
	{
		if (threadShouldExit()) {
			uart.close();
		}
	};
	auto delay = sequence.getDelay();
	uart.open(1170, 8, UART::StopBit::oneStopBit, UART::Parity::noParity, 3);
	wait(100);
	for (int i = 0; i < sequence.getSize(); i++) {
		const auto& step = sequence[i];
		uart.addByte(0x02);				// début séquence
		uart.addByte(step.order);		// ordre
		uart.addByte(step.adress);		// adresse module
		uart.addByte(step.character);	// caractère à afficher
		uart.addByte(0x03);				// fin séquence
		uart.send();
		DBG(0x02 << step.order << step.adress << step.character << 0x03);
		wait(delay);
		progression = i / (sequence.getSize() - 1);
		check();
	}
}
