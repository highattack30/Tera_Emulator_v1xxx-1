#ifndef WORLDSYSTEM_H
#define WORLDSYSTEM_H

#include <mutex>
#include <concurrent_queue.h>
#include <thread>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#define AREA_UPDATE_TIMEOUT 100

class Player; class Account; class Area; class WorldPosition; class Client; class WorldObject; class Creature; class Projectile; class EffectZone; class HuntingZone;
class WorldSystem
{
public:

	static const bool InitializeWorldSystem(const char* areasFilePath, const char * creaturesFilePath, const char* objectsFilePath);

	static const bool EnterWorld(Client* player);


	static void StartSystem();
	static void ShutdownSystem();
	static void ReleaseData();

	static const int GetNearClients(WorldPosition* _point, Area* _area, int range, std::vector<Client*>& _outClients);
	static const int GetNearWorldObjects(WorldPosition* _point, Area* _area, int range, std::vector<WorldObject*>& _outObjects);
	static const int GetNearCreatures(WorldPosition* _point, Area* _area, int range, std::vector<Creature*>& _outCreatures);

	static void OnMovePlayer(Client * client, float x, float y, float z, float x2, float y2, float z2, int heading, int type, short speed, int time);
	static void NewPlayerPosition(Player * player);
	static Area* GetArea(int areaId);

	static void Main(Area* instance);
private:
	static const bool LoadAreas(const char* areasFilePath);
	static const bool LoadCreatures(const char* creturesFilePath);
	static const bool LoadObjects(const char* objectsFilePath);
	static Creature* ResolveCreature(int id);
	static WorldObject * ResolveWorldObject(int id);
	static EffectZone* ResolveEffectZone(int id, Area* area);

	static std::vector<Area*> _areas;
	static std::vector<Creature*> _creatures;
	static std::vector<WorldObject*> _objects;
	static std::mutex _mainLock;


};

#endif