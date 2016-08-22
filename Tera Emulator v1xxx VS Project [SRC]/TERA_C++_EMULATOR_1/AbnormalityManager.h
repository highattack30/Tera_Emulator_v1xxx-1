#ifndef ABNORMALITY_MANAGER_H
#define ABNORMALITY_MANAGER_H

#include <vector>
#include <thread>
#include <mutex>
#include <concurrent_queue.h>

#include "ServerTimer.h"

class IAbnormality; class Client;
class AbnormalityManager
{
public:
	AbnormalityManager();
	~AbnormalityManager();
	const bool Initialize(Client *c);
	void UnInitialize(Client* c);
	void Add(IAbnormality * ab);
	void Release();
private:
	static void Update(AbnormalityManager * instance);
private:
	std::thread _managerThread;
	bool _run;
	std::mutex _addMutex;
	Client * _client;
	std::vector<IAbnormality *> _abonormalities;
	concurrency::concurrent_queue<IAbnormality*> _toAdd;
};
#endif

