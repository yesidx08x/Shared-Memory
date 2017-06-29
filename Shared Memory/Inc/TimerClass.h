#ifndef TIMERCLASS_H
#define TIMERCLASS_H

class TimerClass
{
private:
	float secondsPerCount = 0.0f;
	float currentDeltaTime = 0.0f;
	float frametime = 0.0f;
	float frameLockTime = 0.0f;

	__int64 previousTime = 0;
	__int64 currentTime = 0;

	bool frameLock = false;
	bool frameRun = false;

public:
	TimerClass();
	TimerClass(float lockTime, bool frameLock = true);
	~TimerClass();

	float frameDeltaTime();
	float fps();

	void Tick();
	bool FrameRun();
};
#endif