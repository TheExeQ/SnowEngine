#include "Time.h"

std::chrono::time_point<std::chrono::steady_clock> Time::myStart, Time::myLastFrame;
std::chrono::duration<double> Time::myDeltaTime, Time::myTotalTime;

Time::Time()
{
	myStart = std::chrono::high_resolution_clock::now();
	myLastFrame = myStart;
	myDeltaTime = std::chrono::duration<double>(0);
	myTotalTime = std::chrono::duration<double>(0);
}

void Time::Update()
{
	myTotalTime = std::chrono::high_resolution_clock::now() - myStart;
	myDeltaTime = std::chrono::high_resolution_clock::now() - myLastFrame;
	myLastFrame = std::chrono::high_resolution_clock::now();
}

float Time::GetDeltaTime()
{
	return myDeltaTime.count();
}

double Time::GetTotalTime()
{
	return myTotalTime.count();
}