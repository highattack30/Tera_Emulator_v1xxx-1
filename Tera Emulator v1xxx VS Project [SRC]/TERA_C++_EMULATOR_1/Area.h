#ifndef AREA_H
#define AREA_H

#include <thread>
#include <concurrent_queue.h>
#include <mutex>
#include <string>
#include <vector>

#define CLIENT_LOCK_TIMEOUT 100
class WorldSystem; class Client; class Player; class Account; class Creature; class WorldObject; class Projectile; class EffectZone; class HuntingZone; class WorldPosition;



class Area
{
	friend class GeoService;
	friend class WorldSystem;
public:
	
private:
	Area();
	~Area();

	const bool Initialize();

	const bool Lock();
	void Unlock();

	void ChangeArea(Client * client, Area * area);
	void CloseArea();
	void EnterArea(Client * client);
	void ExitArea(Client * client);
	void SpawnMe(Client* client, WorldPosition* position);
	bool _run;
	bool _running;
	bool _edit;
	std::string _name;
	int _chanel;
	
	WorldPosition* _startPosition;
	concurrency::concurrent_queue<Client*> _toAdd;
	std::thread _areaThread;

	std::vector<Client*> _clients;
	std::vector<Creature*> _npcs;
	std::vector<WorldObject*> _worldObjects;
	std::vector<Projectile*> _projectiles;
	std::vector<EffectZone*> _effectsZones;
	std::vector<HuntingZone*>_huntingZones;
	
	std::mutex _lockMutex;
};


#endif