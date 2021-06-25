/*
  ==============================================================================

	SerialThread.h
	Created: 23 Jun 2021 10:29:11am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Sequence.h"
#include "UART.h"

class Stopwatch
{
	using Clock = std::chrono::high_resolution_clock;

private:
	Clock::time_point start_point;

public:
	Stopwatch() :
		start_point(Clock::now())
	{}

	void reset() { start_point = Clock::now(); }

	double elapsed_time() const
	{
		std::atomic_thread_fence(std::memory_order_relaxed);
		auto counted_time = std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - start_point).count();
		std::atomic_thread_fence(std::memory_order_relaxed);
		return static_cast<double>(counted_time);
	}
	int elapsed_time_ms() const
	{
		std::atomic_thread_fence(std::memory_order_relaxed);
		auto counted_time = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start_point).count();
		std::atomic_thread_fence(std::memory_order_relaxed);
		return static_cast<int>(counted_time);
	}
	void wait(double seconds)
	{
		/*Clock::time_point start = Clock::now();
		while (std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - start).count() < seconds);*/
		std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
	}
	void wait_ms(double milliseconds)
	{
		wait(milliseconds / 1000.0);
	}
};

struct ErrModule
{
	bool err_ok = 0;
	bool erreurs[9] = { 0 };
	enum {
		err_A = 0,
		err_B = 1,
		err_C = 2,
		err_D = 3,
		err_E = 4,
		err_F = 5,
		err_G = 6,
		err_reponse = 7,
		err_illisible = 8
	};
};

class SerialThread : public Thread
{
public:
	SerialThread();
	SerialThread(const Sequence& sequence);
	void setSequence(const Sequence& sequence);
	float getProgression() const;
	void askToExit() { exitAsked = true; }
	ErrModule getModuleResponse(int digitNumber, int timeout_ms);
private:
	bool exitAsked;
	float progression;
	int timeout_ms;
	virtual void run() override;
	Stopwatch s;
	UART uart;
	Sequence sequence;
};