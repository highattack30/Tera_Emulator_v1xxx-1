#include "StatsService.h"
#include "Creature.h"
#include "Player.hpp"
#include "Client.hpp"
#include "Account.hpp"
#include "CreatureStats.h"
#include "CreatureEnum.h"


CreatureStats * StatsService::GetCreatureBaseStats(Creature * creature)
{
	CreatureStats * stats = new CreatureStats();
	memset((void*)stats, 0, sizeof CreatureStats);


	switch (creature->_type)
	{
	case MONSTER:
	{
		MonsterType mType = (MonsterType)creature->_subType;
		switch (mType)
		{

		}

	}break;
	case NPC:
	{
		NpcType nType = (NpcType)creature->_subType;
		switch (nType)
		{
		case BANKER:
			stats->_gender = PlayerGender::MALE;
			stats->_race = PlayerRace::HUMAN;
		case TRADEBROKER:
			stats->_gender = PlayerGender::MALE;
			stats->_race = PlayerRace::POPORI;
		case SHOP:
			stats->_gender = PlayerGender::MALE;
			stats->_race = PlayerRace::HUMAN;
		case SPECIALSHOP:
		{
			stats->_gender = PlayerGender::MALE;
			stats->_race = PlayerRace::HUMAN;
			stats->_maxHp = 1;
			stats->_maxMp = 1;
			stats->_movementSpeed = 100;
			stats->_attackSpeed = 100;
			stats->_critPower = 2;
			stats->_attack = 1;
			stats->_defense = 1;

			stats->reaction = CreatureReaction::Friendly;
			stats->status = CreatureStatus::Alive;
		}break;
		}
	}
	}

	return stats;
}

CreatureStats * StatsService::GetPlayerStartStats()
{
	return nullptr;
}
