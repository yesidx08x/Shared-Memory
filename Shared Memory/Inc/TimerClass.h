#ifndef TIMERCLASS_H
#define TIMERCLASS_H

class TimerClass
{
private:
	double secondsPerCount;
	double currentDeltaTime;
	double frametime;
	double frameLockTime;

	__int64 baseTime;
	__int64 pausedTime;
	__int64 previousTime;
	__int64 stoppedTime;
	__int64 currentTime;

	bool stopped;
	bool frameLock;
	bool frameRun;

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