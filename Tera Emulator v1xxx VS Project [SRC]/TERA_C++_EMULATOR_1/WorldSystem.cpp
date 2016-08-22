#include "WorldSystem.h"
#include "Area.h"
#include "Account.hpp"
#include "Client.hpp"
#include "Player.hpp"
#include "WorldObject.h"
#include "Creature.h"
#include "PlayerEnums.h"
#include "HuntingZone.h"
#include "EffectZone.h"
#include "Projectile.h"
#include "StatsService.h"

#include "FileStream.h"

const bool WorldSystem::LoadAreas(const char* areasFilePath)
{
	int count = -1;
	FileStream* s = new  FileStream(areasFilePath);
	if (!s->IsOpen())
		return false;

	count = s->ReadInt32();
	for (int i = 0; i < count; i++)
	{
		float x = s->ReadFloat();
		float y = s->ReadFloat();
		float z = s->ReadFloat();
		short h = s->ReadInt16();
		int areaId = s->ReadInt32();

		Area * newA = new Area();

		newA->_edit = s->ReadByte() > 0 ? true : false;
		newA->_name = s->ReadASCIIString();


		int npcCount = s->ReadInt32();
		for (int i = 0; i < npcCount; i++)
			newA->_npcs.push_back(ResolveCreature(s->ReadInt32())); //NPCs as creatures

		int huntingZoneCount = s->ReadInt32();
		for (int i = 0; i < huntingZoneCount; i++)
		{
			HuntingZone * newZ = new HuntingZone();
			newZ->_id = s->ReadInt32();
			newZ->_top = s->ReadInt32();
			newZ->_left = s->ReadInt32();
			newZ->_bottom = s->ReadInt32();
			newZ->_right = s->ReadInt32();
			newZ->_monsterType = s->ReadInt32();
			newZ->_isDangerZone = s->ReadByte();

			newA->_huntingZones.push_back(newZ);
		}

		int objectCount = s->ReadInt32();
		for (int i = 0; i < objectCount; i++)
		{
			newA->_worldObjects.push_back(ResolveWorldObject(s->ReadInt32()));
		}

		int effectZoneCount = s->ReadInt32();
		for (int i = 0; i < effectZoneCount; i++)
		{
			newA->_effectsZones.push_back(ResolveEffectZone(s->ReadInt32(), newA));
		}

		newA->_startPosition = new WorldPosition(x, y, z,
			h, areaId);


		if (!newA->Initialize())
		{
			delete newA;
			newA = 0;
		}

		for (size_t j = 0; j < 1; j++)
		{
			newA->_chanel = j + 1;
			_areas.push_back(newA);
		}

	}
	delete s;
	s = 0;
	return true;
}

const bool WorldSystem::LoadCreatures(const char * creturesFilePath)
{
	FileStream  file = FileStream(creturesFilePath);
	if (!file.IsOpen())
		return false;

	int count = file.ReadInt32();
	for (size_t i = 0; i < count; i++)
	{
		Creature * nC = new Creature();
		nC->_id = file.ReadInt32();
		nC->_type = (CreatureType)file.ReadInt32();
		nC->_subType = file.ReadInt32();

		nC->_stats = StatsService::GetCreatureBaseStats(nC);

		_creatures.push_back(nC);
	}

	return true;
}

const bool WorldSystem::LoadObjects(const char * objectsFilePath)
{
	return false; //todo
}

Creature * WorldSystem::ResolveCreature(int id)
{
	for (size_t i = 0; i < _creatures.size(); i++)
	{
		if (_creatures[i] && _creatures[i]->_id == id)
			return _creatures[i]->Clone();
	}
	return nullptr;
}

WorldObject * WorldSystem::ResolveWorldObject(int id)
{
	for (size_t i = 0; i < _objects.size(); i++)
	{
		if (_objects[i] && _objects[i]->_id == id)
			return _objects[i]->Clone();
	}

	return nullptr;
}

EffectZone * WorldSystem::ResolveEffectZone(int id, Area* area)
{
	return nullptr;
}

const bool WorldSystem::InitializeWorldSystem(const char * areasFilePath, const char * creaturesFilePath, const char * objectsFilePath)
{
	if (!LoadAreas(areasFilePath))
		return false;
	/*if (!LoadCreatures(creaturesFilePath))
		return false;
	if (!LoadObjects(objectsFilePath))
		return false;*/

	return true;
}

const bool WorldSystem::EnterWorld(Client * player)
{
	Area * a = nullptr;
	for (int i = 0; i < _areas.size(); i++)
	{
		if (_areas[i] && _areas[i]->_startPosition->_areaId == player->_account->_selectedPlayer->_position->_areaId)
		{
			a = _areas[i];
			break;
		}
	}
	if (a)
	{
		while (!a->Lock())
			continue;
		a->EnterArea(player);
		a->Unlock();

		return true;
	}

	return false;
}

void WorldSystem::StartSystem()
{
	for (size_t i = 0; i < _areas.size(); i++)
	{
		if (_areas[i])
		{
			_areas[i]->_run = true;
			_areas[i]->_areaThread = std::thread(Main, _areas[i]);
			_areas[i]->_areaThread.detach();
		}
	}
}

void WorldSystem::ShutdownSystem()
{
	for (size_t i = 0; i < _areas.size(); i++)
	{
		if (_areas[i])
		{
			_areas[i]->CloseArea();
		}
	}
}

void WorldSystem::ReleaseData()
{
	int t = 0;
	for (size_t i = 0; i < _areas.size(); i++)
	{
		if (_areas[i] && !_areas[i]->_running)
		{
			t = 0;
			delete _areas[i];
			_areas[i] = 0;
		}
		else if (_areas[i] && _areas[i]->_running)
		{
			t++;
			_areas[i]->CloseArea();
			if (t < 100)
				i--;
			else
				t = 0;
			continue;
		}
	}
}


const int WorldSystem::GetNearClients(WorldPosition * _point, Area * _area, int range, std::vector<Client*>& _outClients)
{
	int c = 0;
	for (size_t i = 0; i < _area->_clients.size(); i++)
	{
		if (_area->_clients[i]->_account->_selectedPlayer->_position->DistanceTo(_point) <= (double)range)
		{
			_outClients.push_back(_area->_clients[i]);
			c++;
		}
	}
	return c;
}

const int WorldSystem::GetNearWorldObjects(WorldPosition * _point, Area * _area, int range, std::vector<WorldObject*>& _outObjects)
{
	return 0;
}

const int WorldSystem::GetNearCreatures(WorldPosition * _point, Area * _area, int range, std::vector<Creature*>& _outCreatures)
{
	return 0;
}

void WorldSystem::OnMovePlayer(Client * client, float x, float y, float z, float x2, float y2, float z2, int heading, int type, short speed, int time)
{
	_mainLock.lock();

	_mainLock.unlock();
}

void WorldSystem::NewPlayerPosition(Player * player)
{
	player->_position = new WorldPosition(0, 0, 0, 0, 0); //todo ...
	Area * a = GetArea(1);
	if (a)
		player->_position = a->_startPosition;
}

Area * WorldSystem::GetArea(int areaId)
{
	Area * _out = nullptr;

	for (size_t i = 0; i < _areas.size(); i++)
	{
		if (_areas[i] && _areas[i]->_startPosition->_areaId == areaId)
		{
			_out = _areas[i];
			break;
		}
	}

	return _out;
}

void WorldSystem::Main(Area * instance)
{
	//area thread function
	instance->_running = true;
	int t = 0; int j = 0;
	while (instance->_run)
	{
		Client* nC = 0; //new client, that entered area
		while (instance->_toAdd.try_pop(nC))
			instance->_clients.push_back(nC);


		while (!instance->Lock())
			continue;
		//todo: add distance check [visibleRange]
		// spawn/despawn players by visitedSection 
		for (size_t i = 0; i < instance->_clients.size(); i++)
		{
			if (instance->_clients[i] && instance->_clients[i]->_account)
			{
				Player * me = instance->_clients[i]->_account->_selectedPlayer;
				Player * you = 0;
				for (size_t j = 0; j < instance->_clients.size(); j++)
				{
					if (instance->_clients[j] != instance->_clients[i])
					{
						you = instance->_clients[j]->_account->_selectedPlayer;

						if (me->_currentVisitedSection == you->_currentVisitedSection) //if me and you are in same section,
						{
							if (!instance->_clients[i]->IsVisibleClient(instance->_clients[j]))
							{
								instance->_clients[i]->AddVisibleClient(instance->_clients[j]); //add you to me [spawn]
								instance->_clients[j]->AddVisibleClient(instance->_clients[i]); //add me to you [spawn]
							}
						}
						else if (me->_currentVisitedSection != you->_currentVisitedSection) //if me/you just leved section
						{
							if (instance->_clients[i]->IsVisibleClient(instance->_clients[j]))
							{
								instance->_clients[i]->RemoveVisibleClient(instance->_clients[j]); //remove you from me [despwn]
								instance->_clients[j]->RemoveVisibleClient(instance->_clients[i]); //remove me from you [despwn]
							}
						}
					}
				}
			}
			else
			{
				continue;
			}
		}


		for (size_t i = 0; i < instance->_huntingZones.size(); i++)
		{
			if (instance->_huntingZones[i])
			{
				while (!instance->_huntingZones[i]->LockZone())
				{
					t++;
					if (t >= 50)
					{
						break;
					}
				}
				if (t >= 100)
					continue;

				instance->_huntingZones[i]->Action();
				instance->_huntingZones[i]->UnlockZone();
			}
		}

		for (size_t i = 0; i < instance->_effectsZones.size(); i++)
		{
			if (instance->_effectsZones[i])
			{
				while (!instance->_effectsZones[i]->LockEffectZone())
				{
					t++;
					if (t >= 50)
					{
						break;
					}
				}
				if (t >= 100)
					continue;

				instance->_effectsZones[i]->Action();
				instance->_effectsZones[i]->UnlockEffectZone();
			}
		}

		for (size_t i = 0; i < instance->_projectiles.size(); i++)
		{
			if (instance->_projectiles[i])
			{
				while (!instance->_projectiles[i]->Lock())
				{
					t++;
					if (t >= 50)
					{
						break;
					}
				}
				if (t >= 100)
					continue;

				instance->_projectiles[i]->Action();
				instance->_projectiles[i]->Unlock();
			}
		}

		for (size_t i = 0; i < instance->_worldObjects.size(); i++)
		{
			if (instance->_worldObjects[i])
			{
				while (!instance->_worldObjects[i]->Lock())
				{
					t++;
					if (t >= 50)
					{
						break;
					}
				}
				if (t >= 100)
					continue;

				instance->_worldObjects[i]->Action();
				instance->_worldObjects[i]->Unlock();
			}
		}

		instance->Unlock();


		Sleep(10);
	}

	instance->_running = false;
}


std::vector<Area*> WorldSystem::_areas;
std::vector<Creature*>  WorldSystem::_creatures;
std::vector<WorldObject*>  WorldSystem::_objects;
std::mutex WorldSystem::_mainLock;