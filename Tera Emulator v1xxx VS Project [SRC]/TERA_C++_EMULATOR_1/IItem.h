#ifndef IITEM_H
#define IITEM_H
#include <string>
#include <vector>

#include "Crypt\data_type.hpp"
#include "PlayerEnums.h"
#include "ItemEnum.h"


class IItem
{
	struct Abnormality
	{
		int id;
		float value;
	};
public:
	IItem();
	~IItem();

	int operator ()(int);
	IItem * Clone();

	int _id;
	std::string _name;
	ItemType _type;
	int _itemLevel;
	int _requiredLevel;
	int _maxStack;
	byte _tradable;
	ItemRareGrade _rareGrade;
	byte _sellable;
	int _sellPrice;
	byte _static;
	byte _warehouseStoreable;
	byte _usable;
	byte _useOnlyByGender;
	byte _useOnlyByRace;
	byte _bankStorable;
	byte _guildBankStorable;
	byte _useOnlyByClass;
	std::vector<int> _requiredClassList;
	int _stackCount;
	int _feedStock;
	int _crystals[4];
	byte _hasBonus;
	std::vector<int> _bonusList;
	int _buyPrice;
	int _coolTime;
	byte _enchantEnabled;
	ItemGrade _itemGrade;
	int _enchantLevel;
	byte _masterworkGrade;
	int _attackModifier;
	int _defenseModifier;
	byte _gender;
	byte _hasEffect;
	std::vector<int> _effectList;
};
#endif
