/*
  ==============================================================================

	SerialThread.h
	Created: 23 Jun 2021 10:29:11am
	Author:  Eloi GUIHARD-GOUJON

	Contient les classes n�cessaires � l'envoi d'ordres en s�rie via un thread � part

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Sequence.h"
#include "UART.h"
#include "Core.h"

/// <summary>
/// H�riter de cette classe pour donner une notion de progression � un thread.
/// Utiliser les methodes setProgression() dans le thread pour faire �voluer la progression.
/// </summary>
class ProgressionThread : public Thread
{
public:
	ProgressionThread() : Thread("ProgressionThread"), progression(0.0f), exitAsked(false) {}

	// Retourne la progoression actuelle du thread
	float getProgression() const { return progression; }

	// Demande au thread de se fermer
	void askToExit() { exitAsked = true; }

protected:
	void setProgression(float newProgression) { progression = newProgression; }
	bool exitAsked;

private:
	float progression;
};

/// <summary>
/// Thread qui permet de transmettre en s�rie des ordres via une s�quence.
/// Ce thread est lanc� via la m�thode startThread()
/// </summary>
class SerialThread : public ProgressionThread
{
public:
	SerialThread();
	SerialThread(const Sequence& sequence);

	void setSequence(const Sequence& sequence);

	// Retourne le temps maximal qu'attend le thread pour avoir une r�ponse du module
	int getTimeout_ms(unsigned char ordre);

	// Choisir si le thread doit attendre une r�ponse du module avant d'envoyer l'ordre au module suivant
	void setWaitForResponse(bool shouldWaitForResponse) { waitForResponse = shouldWaitForResponse; }

private:
	// M�thode appel�e lorsque startThread() est appel�
	virtual void run() override;

	// Retourne le code erreur qu'a r�pondu
	ErrModule getModuleResponse(int digitNumber, int timeout_ms);

	int timeout_ms;
	bool waitForResponse;
	Stopwatch s;
	UART uart;
	Sequence sequence;
};