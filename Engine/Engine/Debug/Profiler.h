#pragma once
#include <chrono>

namespace Snow
{
	class Profiler
	{
	public:
		void Start();
		void Stop();
		void Reset();
		double GetTime();

	private:
		std::chrono::time_point<std::chrono::steady_clock> myStart, myStop;
	};

	void Profiler::Start()
	{
		myStart = std::chrono::high_resolution_clock::now();
		myStop = myStart;
	}

	void Profiler::Stop()
	{
		myStop = std::chrono::high_resolution_clock::now();
	}

	double Profiler::GetTime()
	{
		return (myStop - myStart).count();
	}
}
