#ifndef TIMERCLASS_H
#define TIMERCLASS_H

class TimerClass
{
private:
	double secondsPerCount = 0.0;
	double currentDeltaTime = 0.0;
	double frametime = 0.0;
	double frameLockTime = 0.0;

	__int64 baseTime = 0;
	__int64 pausedTime = 0;
	__int64 previousTime = 0;
	__int64 stoppedTime = 0;
	__int64 currentTime = 0;

	bool stopped = false;
	bool frameLock = false;
	bool frameRun = false;

public:
	//Initializing the Timer.
	TimerClass();
	TimerClass(float lockTime, bool frameLock = true);
	~TimerClass();


	float time();
	float realDeltaTime();
	float frameDeltaTime();
	int fps();

	void Tick();
	void Reset();
	void Start();
	void Stop();
	bool FrameRun();

	void SetFrameLock(int lockTime);
};
#endif