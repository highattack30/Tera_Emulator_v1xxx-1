#include "AbnormalityManager.h"
#include "Client.hpp"
#include "Player.hpp"
#include "Account.hpp"
#include "IAbnormality.h"


AbnormalityManager::AbnormalityManager()
{
	_client = 0;
}


AbnormalityManager::~AbnormalityManager()
{
	_client = 0;
	_toAdd.clear();
	for (size_t i = 0; i < _abonormalities.size(); i++)
	{
		delete _abonormalities[i];
		_abonormalities[i] = 0;
	}
	_abonormalities.clear();
}

const bool AbnormalityManager::Initialize(Client * c)
{
	if (!c)
		return false;

	_managerThread = std::thread(Update, this);
	_managerThread.detach();

	return true;
}

void AbnormalityManager::UnInitialize(Client * c)
{
	_run = false;
}

void AbnormalityManager::Add(IAbnormality * ab)
{
	_addMutex.lock();
	_toAdd.push(ab);
	_addMutex.unlock();
}

void AbnormalityManager::Release()
{
	_run = false;
}

void AbnormalityManager::Update(AbnormalityManager * _instance)
{
	while (_instance && _instance->_run)
	{
		ServerTimer::ServerTime time = ServerTimer::GetTime();

		IAbnormality *ab = nullptr;
		IAbnormality * an = nullptr;
		while (_instance->_toAdd.try_pop(an))
		{
			ab = an->Clone();
			ab->_alive = true;
			ab->_currentTime = 0.0f;
			ab->_startTime = time.totalTime;
			_instance->_abonormalities.push_back(ab);
		}


		for (size_t i = 0; i < _instance->_abonormalities.size(); i++)
		{
			IAbnormality *k = _instance->_abonormalities[i];
			if (k)
			{
				if (!k->_alive && _instance)
				{
					k = 0;
					delete _instance->_abonormalities[i];
					_instance->_abonormalities[i] = 0;
					continue;
				}
				else if (_instance)
				{
					k->_currentTime += time.elapsedTime;
					if ((k->_currentTime - k->_startTime) >= k->_lifeTime)
						k->_alive = false;

					if (k->_alive)
					{
						k->ApplyAbnormality(_instance->_client);
					}
				}

				if (!_instance)
					break;
			}
			else
				_instance->_abonormalities.erase(_instance->_abonormalities.begin() + i);
		}

	}

	if (_instance)
	{
		delete _instance;
		_instance = 0;
	}
}