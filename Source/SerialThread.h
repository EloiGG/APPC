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
#include "Core.h"

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
		std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
	}
	void wait_ms(double milliseconds)
	{
		wait(milliseconds / 1000.0);
	}
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
	int getTimeout_ms(unsigned char ordre);
	void setWaitForResponse(bool shouldWaitForResponse) { waitForResponse = shouldWaitForResponse; }
private:
	bool exitAsked;
	float progression;
	int timeout_ms;
	bool waitForResponse;
	virtual void run() override;
	Stopwatch s;
	UART uart;
	Sequence sequence;
};