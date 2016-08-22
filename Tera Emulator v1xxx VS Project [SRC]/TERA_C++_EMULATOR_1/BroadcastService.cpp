#include "BroadcastService.h"
#include "Client.hpp"
#include "Stream.h"

BroadcastSystem::BroadcastSystem()
{
	_run = true;
	_mainThread = std::thread(Main, this);
	_mainThread.detach();
}

BroadcastSystem::~BroadcastSystem()
{
	ToSendPacket * packet = 0;
	while (_toSendList.try_pop(packet))
	{
		if (packet)
			delete packet;
		packet = 0;
	}

	_toSendList.clear();
}

const bool BroadcastSystem::InitBroadcastService(unsigned short threadCount)
{
	_instance = new BroadcastSystem();
	return true;
}

void BroadcastSystem::Release()
{
	_instance->_run = false;
}

void BroadcastSystem::Broadcast(Client* caller, Stream* s, BC_TYPE type, void** args)
{
	_instance->_toSendList.push(new ToSendPacket(caller, new Stream(s->_raw, s->_size), type, true, args));
	_instance->_wait_Lock.notify_one();
}

void BroadcastSystem::Main(BroadcastSystem * instance)
{
	ToSendPacket * packet = 0;
	while (instance->_run)
	{
		if (instance->_toSendList.empty())
		{
			instance->_wait_Lock.wait(std::unique_lock<std::mutex>(instance->_locker));
		}

		while (instance->_run && instance->_toSendList.try_pop(packet))
		{
			if (packet)
			{

				switch (packet->_bcType)
				{
				case ME:
				{
					packet->_callerClient->Send(packet->_data);

				}break;
				case VISIBLE_CLIENTS :
				{

					for (size_t i = 0; i < packet->_callerClient->_visibleClients.size(); i++)
					{
						if (packet->_callerClient->_visibleClients[i])
						{
							packet->_callerClient->_visibleClients[i]->Send(packet->_data);
						}
					}


				}break;

				default:
					break;
				}

				delete packet;
				packet = 0;
			}
		}
	}


	delete _instance;
	_instance = 0;
}

void BroadcastSystem::Detach()
{
	_mainThread.detach();
}

BroadcastSystem * BroadcastSystem::_instance = 0;
