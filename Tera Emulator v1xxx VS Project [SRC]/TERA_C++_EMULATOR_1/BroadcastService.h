#ifndef BROADCAST_SERVICE_H
#define BROADCAST_SERVICE_H

#include <thread>
#include <vector>
#include <mutex>
#include <concurrent_queue.h>

#include "Stream.h"
#include "ToSendPacket.h"

class BroadcastSystem
{
public:
	BroadcastSystem();
	~BroadcastSystem();
	static const bool InitBroadcastService(unsigned short threadCount);
	static void Release();
	static void Broadcast(Client* caller, Stream* s, BC_TYPE type, void** args);

private:
	static void Main(BroadcastSystem * instance);

	void Detach();

	bool _run;
	static BroadcastSystem * _instance;
	std::thread _mainThread;
	concurrency::concurrent_queue<ToSendPacket*> _toSendList;
	std::mutex _locker;
	std::condition_variable _wait_Lock;
};


#endif


