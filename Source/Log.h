/*
  ==============================================================================

	Log.h
	Created: 28 Jun 2021 8:56:29am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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

class Log
{
public:
	static constexpr int DEBUG_LEVELS = 2;

	Log(const Log&) = delete;
	static void writeLn(const String& string);
	static void writeNext(const String& string);
	void writeImpl(const String& string);
	
	static void update();
	static void title(const String& string);
	static void ln(int debugLevel = 1, int numberOfLines = 1);
	static Log& get() { return _log; }
	std::function<void()> updateFunction;
	void updateImpl();
	int getTime_ms() { return s.elapsed_time_ms(); }

private:
	const static int textWidth = 60;
	Log();
	bool init;
	static Log _log;
	File logFile;
	Stopwatch s;
};