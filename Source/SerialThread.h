/*
  ==============================================================================

	SerialThread.h
	Created: 23 Jun 2021 10:29:11am
	Author:  Eloi GUIHARD-GOUJON

	Contient les classes nécessaires à l'envoi d'ordres en série via un thread à part

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Sequence.h"
#include "UART.h"
#include "Core.h"

/// <summary>
/// Hériter de cette classe pour donner une notion de progression à un thread.
/// Utiliser les methodes setProgression() dans le thread pour faire évoluer la progression.
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
/// Thread qui permet de transmettre en série des ordres via une séquence.
/// Ce thread est lancé via la méthode startThread()
/// </summary>
class SerialThread : public ProgressionThread
{
public:
	SerialThread();
	SerialThread(const Sequence& sequence);

	void setSequence(const Sequence& sequence);

	// Retourne le temps maximal qu'attend le thread pour avoir une réponse du module
	int getTimeout_ms(unsigned char ordre);

	// Choisir si le thread doit attendre une réponse du module avant d'envoyer l'ordre au module suivant
	void setWaitForResponse(bool shouldWaitForResponse) { waitForResponse = shouldWaitForResponse; }

private:
	// Méthode appelée lorsque startThread() est appelé
	virtual void run() override;

	// Retourne le code erreur qu'a répondu
	ErrModule getModuleResponse(int digitNumber, int timeout_ms);

	int timeout_ms;
	bool waitForResponse;
	Stopwatch s;
	UART uart;
	Sequence sequence;
};