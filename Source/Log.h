/*
  ==============================================================================

	Log.h
	Created: 28 Jun 2021 8:56:29am
	Author:  Eloi GUIHARD-GOUJON

	Contient les classes nécessaires à créer un fichier log à la fermeture du logiciel.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/// <summary>
/// Chronomètre
/// </summary>
class Stopwatch
{
	using Clock = std::chrono::high_resolution_clock;

private:
	Clock::time_point start_point;

public:
	Stopwatch() :
		start_point(Clock::now())
	{}

	// Réinitialise le chronomètre
	void reset() { start_point = Clock::now(); }

	// Retourne le nombre de secondes passées depuis le dernier appel de reset()
	double elapsed_time() const
	{
		std::atomic_thread_fence(std::memory_order_relaxed);
		auto counted_time = std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - start_point).count();
		std::atomic_thread_fence(std::memory_order_relaxed);
		return static_cast<double>(counted_time);
	}

	// Retourne le nombre de millisecondes passées depuis le dernier appel de reset()
	int elapsed_time_ms() const
	{
		std::atomic_thread_fence(std::memory_order_relaxed);
		auto counted_time = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start_point).count();
		std::atomic_thread_fence(std::memory_order_relaxed);
		return static_cast<int>(counted_time);
	}

	// Met en pause le thread actuel pendant le nombre de secondes spécifiées
	void wait(double seconds)
	{
		std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
	}

	// Met en pause le thread actuel pendant le nombre de millisecondes spécifiées
	void wait_ms(double milliseconds)
	{
		wait(milliseconds / 1000.0);
	}
};

/// <summary>
/// Singleton permettant l'écriture d'un fichier log au fur et à mesure que le programme s'exécute
/// </summary>
class Log
{
public:
	Log(const Log&) = delete;

	// Ecrit une ligne en incluant le temps d'éxecution
	static void writeLn(const String& string);

	// Ecrit à la suite de la ligne en cours d'écriture
	static void writeNext(const String& string);

	// Fonction obsolète, ne pas utiliser
	static void update();

	// Permet d'écrire un titre
	static void title(const String& string);

	// Revient à la ligne
	static void ln();

	// Les fonction suivantes ne sont pas statiques et ne peuvent pas être utilisées	//
	void _writeImpl(const String& string);												//
	void _updateImpl();																	//
	int _getTime_ms() { return s.elapsed_time_ms(); }									//

private:
	const static int textWidth = 60;

	Log();

	bool init;
	static Log _log;
	File logFile;
	Stopwatch s;
};