#include "TimerClass.h"
#include <Windows.h>

TimerClass::TimerClass(){}

TimerClass::TimerClass(float lockTime, bool frameLock)
{
	currentDeltaTime = -1.0f;
	frameLockTime = 1.0f / lockTime;
	frametime = frameLockTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	previousTime = currentTime;
	this->frameLock = frameLock;

	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	secondsPerCount = 1.0f / countsPerSec;
}

TimerClass::~TimerClass(){}

float TimerClass::frameDeltaTime()
{
	if (frameLock)
		return (float)frameLockTime;
	else
		return (float)currentDeltaTime;
}

float TimerClass::fps(){return 1.0f / frameDeltaTime();}

void TimerClass::Tick()
{
	
	//Check what time this frame happens at
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	//Check the time since the last frame
	currentDeltaTime = (currentTime - previousTime)*secondsPerCount;

	//Make sure that delta isn't negative
	if (currentDeltaTime < 0)
	{
		currentDeltaTime = 0;
	}
	frametime += currentDeltaTime;
	if (frametime >= frameLockTime)
	{
		frameRun = true;
		frametime = 0.0;
	}
	else
		frameRun = false;

	//Swithc "current time" to "previous time"
	previousTime = currentTime;
}

bool TimerClass::FrameRun()
{
	if (frameLock)
		return frameRun;
	else
		return true;
}