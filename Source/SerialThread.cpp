/*
  ==============================================================================

	Serial.cpp
	Created: 23 Jun 2021 10:29:11am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "SerialThread.h"

SerialThread::SerialThread() : timeout_ms(3000), waitForResponse(true)
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
	setProgression(0.0f);
}

void SerialThread::run()
{
	auto& c = Core::get();
	Log::title("Transmission");
	auto delay = sequence.getDelay();
	Log::write("\nOuverture du port COM");
	Log::write(String(c.getCOMPort()));
	Log::write(" ");
	Log::write("avec baud rate = 1170, 8 bits/byte, un stop bit, pas de parity bit...", 2);
	if (uart.open(1170, 8, UART::StopBit::oneStopBit, UART::Parity::noParity, c.getCOMPort()))
		Log::write("port ouvert !\n");
	else {
		Log::write("erreur lors de l'ouverture\n");
		goto fin;
	}

	wait(100);
	for (int i = 0; i < sequence.getSize(); i++) {
		const auto& step = sequence[i];
		uart.addByte(0x02);				// début séquence
		uart.addByte(step.order);		// ordre
		uart.addByte(step.adress);		// adresse module
		uart.addByte(step.character);	// caractère à afficher
		uart.addByte(0x03);				// fin séquence
		uart.send();

		Log::write(CharPointer_UTF8("\nEnvoi de la requête d'affichage du caractère "));
		Log::write(String(step.character));
		if (step.order == 0x46) Log::write(CharPointer_UTF8(" avec contrôle segment "));
		Log::write(CharPointer_UTF8(" à l'adresse "));
		Log::write(String(step.adress));
		Log::write(String(CharPointer_UTF8(" : Octets envoyés (en hexadécimal) : 02 ")) +
			String(step.order) + String(" ") + String(step.adress) + String(" ") +
			String(step.character) + String(" ") + String("03"), 2);
		Log::ln();
		{
			MessageManagerLock m(this);
			if (m.lockWasGained())
				Log::update();
		}
		if (waitForResponse)
		{
			Log::write(CharPointer_UTF8("Récupération de la réponse du module : "));
			auto response = getModuleResponse(step.adress - 0x30, getTimeout_ms(step.order));
			if (response.err_ok)
				Log::write(CharPointer_UTF8("réponse du module, aucune erreur"));
			else if (response.stopping)
				Log::write(CharPointer_UTF8("arrêt de la séquence"));
			else if (response.erreurs[response.err_illisible])
				Log::write(CharPointer_UTF8("réponse du module illisible !"));
			else if (response.erreurs[response.err_reponse])
				Log::write(CharPointer_UTF8("pas de réponse du module"));
			else
			{
				Log::write("Il y a des erreurs sur les segments suivants : ");
				for (int i = 0; i < 7; i++)
					if (response.erreurs[i] == true) {
						char str[] = "A";
						str[0] += i;
						Log::write(str);
					}
			}
			Log::ln();

			{ // messageThread Lock
				MessageManagerLock m(this);
				if (m.lockWasGained()) {
					c.setModuleState(step.adress - 0x30, response);
					ErrModule r;
					r.work_in_progress = true;
					if (!(threadShouldExit() || exitAsked))
						c.setModuleState(step.adress - 0x30 + 1, r);
					Log::update();
				}
			}
		}
		wait(delay);
		setProgression((float)i / (sequence.getSize() - 1));
		if (threadShouldExit() || exitAsked)
			goto fin;
	}
	Core::get().init();
fin:
	exitAsked = false;
	MessageManagerLock mml(this);
	Log::title("fin de la transmission");
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
		wait(30);
		if (threadShouldExit() || exitAsked) {
			em.err_ok = false;
			em.stopping = true;
			return em;
		}
		uart.read(&c);		//...0x02
		if (s.elapsed_time_ms() > timeout_ms) {
			em.err_ok = false;
			em.erreurs[em.err_reponse] = true;
			return em;
		}
	}
	uart.read(&c);			//0x52
	uart.read(&c);			//<adresse>
	if (c == digitNumber + 0x30) {
		uart.read(&c);		//<code_err>
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

int SerialThread::getTimeout_ms(unsigned char ordre)
{
	int time_out_rep_module = 0;
	switch (ordre)
	{
	case 'P':
		time_out_rep_module = 2;
		break;

	case 'W':
		time_out_rep_module = 2;
		break;

	case 'F':
		time_out_rep_module = 8;
		break;

	case 'A':
		time_out_rep_module = 3;
		break;

	case 'T':
		time_out_rep_module = 8;
		break;


	case 'C':
		time_out_rep_module = 7;
		break;

	case 'I':
		time_out_rep_module = 3;
		break;


	default:
		time_out_rep_module = 8;
		break;

	}
	return time_out_rep_module * 2000;
}
