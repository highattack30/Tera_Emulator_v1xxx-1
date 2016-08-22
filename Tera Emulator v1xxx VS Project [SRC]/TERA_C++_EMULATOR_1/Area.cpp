#include "Area.h"
#include "Client.hpp"
#include "Player.hpp"
#include "Account.hpp"
#include "HuntingZone.h"
#include "EffectZone.h"

Area::Area()
{
	_run = _running = false;
	_edit = false;
}


Area::~Area()
{
	CloseArea();

	for (size_t i = 0; i < _clients.size(); i++)
	{
		_clients[i] = 0;
	}
	_clients.clear();

	for (size_t i = 0; i < _worldObjects.size(); i++)
	{
		if (_worldObjects[i])
		{
			delete _worldObjects[i];
			_worldObjects[i] = 0;
		}
	}
	_worldObjects.clear();

	for (size_t i = 0; i < _projectiles.size(); i++)
	{
		if (_projectiles[i])
		{
			delete _projectiles[i];
			_projectiles[i] = 0;
		}
	}
	_projectiles.clear();

	for (size_t i = 0; i < _effectsZones.size(); i++)
	{
		if (_effectsZones[i])
		{
			delete _effectsZones[i];
			_effectsZones[i] = 0;
		}
	}

	for (size_t i = 0; i < _huntingZones.size(); i++)
	{
		if (_huntingZones[i])
		{
			delete _huntingZones[i];
			_huntingZones[i] = 0;
		}
	}
	_huntingZones.clear();

	if (_startPosition)
	{
		delete _startPosition;
		_startPosition = 0;
	}
}

const bool Area::Initialize()
{

	for (size_t i = 0; i < _huntingZones.size(); i++)
	{
		if (_huntingZones[i] && !_huntingZones[i]->Initialize())
		{
			return false;
		}
	}

	for (size_t i = 0; i < _effectsZones.size(); i++)
	{
		if (_effectsZones[i] && !_effectsZones[i]->Intialize())
			return false;
	}

	return true;
}

const bool Area::Lock()
{
	return _lockMutex.try_lock();
}

void Area::Unlock()
{
	_lockMutex.unlock();
}

void Area::ChangeArea(Client * client, Area * area)
{
	Player * p = 0;
	while (!(p = client->LockPlayer()))
		continue;

	this->ExitArea(client);
	area->EnterArea(client);

	client->UnlockPlayer();
}

void Area::CloseArea()
{
	if (_run)
		_run = false;


	for (size_t i = 0; i < _clients.size(); i++)
	{
		_clients[i]->Close();
		_clients[i] = 0;
	}
	_clients.clear();
}

void Area::EnterArea(Client * client)
{
	client->_account->_selectedPlayer->_currentArea = this;
	Player * p = client->_account->_selectedPlayer;
	
	if (p->_position->_X == 0 && p->_position->_Y == 0 && p->_position->_Z == 0)
		p->_position = _startPosition;
	p->_position->_areaId = _startPosition->_areaId;
	
	_toAdd.push(client);

}


void Area::ExitArea(Client * client)
{
	if (!client->IsLocked())
		return;

	//todo add exit area logic here

	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i] == client)
		{
			_clients.erase(_clients.begin() + i);
			break;
		}
	}

}

void Area::SpawnMe(Client * client, WorldPosition * position)
{
	bool hasArea = false;
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i] == client)
		{
			while (!client->LockPlayer())
				continue;

			ExitArea(client);
			EnterArea(client);

			position->CopyTo(client->_account->_selectedPlayer->_position);

			client->_account->_selectedPlayer->_position->_areaId = _startPosition->_areaId;
			
			client->UnlockPlayer();

		}
	}
}



