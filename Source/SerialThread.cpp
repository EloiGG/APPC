/*
  ==============================================================================

	Serial.cpp
	Created: 23 Jun 2021 10:29:11am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "SerialThread.h"

SerialThread::SerialThread() : Thread("SerialThread"), progression(0.0f), exitAsked(false), timeout_ms(3000)
{
	setPriority(9);
}

SerialThread::SerialThread(const Sequence& s) : SerialThread()
{
	sequence = s;
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

		getModuleResponse(step.adress - 0x30, timeout_ms);

		wait(delay);
		progression = (float)i / (sequence.getSize() - 1);
		if (threadShouldExit() || exitAsked)
			break;
	}
	exitAsked = false;
	MessageManagerLock mml(this);
	uart.close();
	if (mml.lockWasGained())
	{
		Core::get().setInTransmission(false);
		Core::get().updateVisualization();
	}
}

ErrModule SerialThread::getModuleResponse(int digitNumber, int timeout_ms)
{
	ErrModule em;
	s.reset();
	unsigned char c = 0;
	while (c != 0x02) {
		s.wait_ms(30);
		uart.read(&c); //...0x02
		if (s.elapsed_time_ms() > timeout_ms) {
			em.err_ok = false;
			em.erreurs[em.err_reponse] = true;
			return em;
		}
	}
	uart.read(&c);		//0x52
	uart.read(&c);		//<adresse>
	if (c == digitNumber + 0x30) {
		uart.read(&c);	//<code_err>
		if (c == 0) {		// si pas d'erreur
			em.err_ok = true;
			return em;
		}
		else {				// si erreur
			em.err_ok = false;
			for (char b = 0b00000001, d = 'A', i = 0; d <= 'G'; b <<= 1, d++, i++)
				if ((b & c) != 0) {
					em.erreurs[i] = true;
				}
			return em;
		}
	}
	else {
		em.erreurs[em.err_illisible] = true;
		em.err_ok = false;
		return em;
	}
}