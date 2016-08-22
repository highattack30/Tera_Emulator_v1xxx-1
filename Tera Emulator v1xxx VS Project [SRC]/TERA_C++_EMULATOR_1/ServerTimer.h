#ifndef SERVER_TIMER_H
#define SERVER_TIMER_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <mutex>
#include <thread>

class ServerTimer
{
public:
	struct ServerTime
	{
		float elapsedTime;
		float totalTime;

		ServerTime(float f1, float f2)
		{
			elapsedTime = f1;
			totalTime = f2;
		}
	};
public:
	ServerTimer();
	~ServerTimer();
private:
	static const bool Initialize();

	static void Update();
	
	static void UpdateTime(float *f1, float* f2);
public:
	static const bool Start();
	static void Stop();

	static ServerTime GetTime();

	static uint64_t GetCurrentUTC();
private:
	static ServerTimer * _instance;
	bool _run;
	ServerTime * _serverTime;
	LONGLONG _start;
	float _frequencySeconds;
	float _utc;
	std::thread _timerThread;
	std::mutex _getTimeMutex;

};

#endif