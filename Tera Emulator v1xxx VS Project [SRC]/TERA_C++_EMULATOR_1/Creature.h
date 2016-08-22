#ifndef CREATURE_H
#define CREATURE_H

#include <vector>
#include <string>

#include "Entity.h"
#include "WorldPosition.h"

enum CreatureType : int {
	MONSTER = 0,
	NPC = 1,

};
enum MonsterType : int
{

};

enum NpcType : int
{
	BANKER = 1,
	TRADEBROKER = 2,
	SHOP = 3,
	SPECIALSHOP = 4,

};

class WorldSystem; class Client;  class Area; class Stream; class AIElement; class Bonus; class CreatureStats; class AbnormalityManager;
class Creature : public Entity
{
	friend  class WorldSystem;
	friend class StatsService;
	friend class HuntingZone;

	Creature();
	virtual ~Creature();

	void Despawn();
	void Spawn(Area * area, int huntingZoneId, WorldPosition* position);

	void Show(Client *client);
	void Hide(Client *client);

	Creature * Clone();

	WorldPosition  _position;
	Area * _area;

	Stream * _spawnData;
	Stream * _despawnData;

	CreatureStats* _stats;
	bool _spawned;
	CreatureType _type;
	int _subType;
	int _id;
	std::vector<AIElement*> _aiElements;

	AbnormalityManager * _abnormalityManager;
};



#endif