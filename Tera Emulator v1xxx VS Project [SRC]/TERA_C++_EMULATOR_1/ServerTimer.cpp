#include "ServerTimer.h"
#include <chrono>


ServerTimer::ServerTimer()
{
	_run = false;
	_serverTime = new ServerTime(0.0f, 0.0f);
	ZeroMemory(_serverTime, sizeof ServerTime);
	_start = 0;
	_frequencySeconds = 0.0f;
}

ServerTimer::~ServerTimer()
{
	if (_serverTime)
	{
		delete _serverTime;
		_serverTime = 0;
	}
}

const bool ServerTimer::Initialize()
{
	if (!_instance)
		_instance = new ServerTimer();

	LARGE_INTEGER i;
	if (!QueryPerformanceFrequency(&i))
		return false;
	_instance->_frequencySeconds = (float)(i.QuadPart);

	QueryPerformanceCounter(&i);

	_instance->_start = i.QuadPart;
	_instance->_serverTime->elapsedTime = 0;
	_instance->_serverTime->totalTime = 0;

	return true;
}

void ServerTimer::Update()
{
	float f1 = 0.0f;
	float f2 = 0.0f;
	while (_instance && _instance->_run)
	{
		LARGE_INTEGER i;
		QueryPerformanceCounter(&i);
		f1 = (float)(i.QuadPart - _instance->_start) / _instance->_frequencySeconds;
		_instance->_start = i.QuadPart;
		f2 += f1;
		UpdateTime(&f1, &f2);
	}

	delete _instance;
	_instance = 0;
}

uint64_t ServerTimer::GetCurrentUTC()
{
	long long t;
	time(&t);
	return (uint64_t)t;
}

void ServerTimer::UpdateTime(float *f1, float *f2)
{
	_instance->_getTimeMutex.lock();
	_instance->_serverTime->elapsedTime = *f1;
	_instance->_serverTime->totalTime = *f2;
	_instance->_getTimeMutex.unlock();
}

const bool ServerTimer::Start()
{
	if (!_instance && !Initialize())
		return false;

	_instance->_run = true;
	_instance->_timerThread = std::thread(Update);
	_instance->_timerThread.detach();

	return true;
}

void ServerTimer::Stop()
{
	if (_instance)
	{
		_instance->_run = false;
	}
}

ServerTimer::ServerTime ServerTimer::GetTime()
{
	float f1 = 0.0f, f2 = 0.0f;
	_instance->_getTimeMutex.lock();
	f1 = _instance->_serverTime->elapsedTime;
	f2 = _instance->_serverTime->totalTime;
	_instance->_getTimeMutex.unlock();

	return ServerTime(f1, f2);
}


ServerTimer * ServerTimer::_instance = 0;