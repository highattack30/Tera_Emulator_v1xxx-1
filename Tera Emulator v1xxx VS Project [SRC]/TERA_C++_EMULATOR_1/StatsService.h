#pragma once

#include <vector>

class CreatureStats; class Creature;
class StatsService
{
public:

	static CreatureStats* GetCreatureBaseStats(Creature * creature);
	static CreatureStats * GetPlayerStartStats();

};

