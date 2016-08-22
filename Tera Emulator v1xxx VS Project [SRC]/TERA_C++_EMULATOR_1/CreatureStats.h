#pragma once

#include "PlayerEnums.h"

#include <string>
#include <vector>
#include "CreatureEnum.h"

class Bonus;
class CreatureStats
{
public:
	CreatureStats();
	~CreatureStats();

public:

	//npc
	PlayerClass _class;
	PlayerGender _gender;
	PlayerRace _race;
	std::string _name;
	int _huntingZoneId;

	CreatureReaction reaction;
	CreatureStatus status;
	//level
	int _level;

	//HpMp
	int _currentHp;
	int _maxHp;
	int _staminaHp;
		
	int _currentMp;
	int _maxMp;
	int _staminaMp;
		
	//combat
	int _attack;
	int _defense;
	int _impact;
	int _balance;
	float _critChance;
	float _critResist;
	int _critPower;
		
	//stats
	int _power;
	int _endurance;
	float _impactFactor;
	float _balanceFactor;
	int _attackSpeed;
	int _movementSpeed;
		
	//resistance
	float _weakeningResistance;
	float _periodicDamageResistance;
	float _stunResistance;
		
	//regen
	int _naturalMpRegen;
	int _naturalHpRegen;
	int _combatMpRegen;
	int _combatHpRegen;

	//additional stats
	std::vector<Bonus*> _bonusList;

	bool _isClone;

	CreatureStats * Clone();
};

