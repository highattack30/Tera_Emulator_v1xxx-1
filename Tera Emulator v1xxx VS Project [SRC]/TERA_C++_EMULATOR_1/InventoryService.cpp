#include "InventoryService.h"
#include "Client.hpp"
#include "Account.hpp"
#include "Stream.h"
#include "IItem.h"

#include "OpCodesEnum.h"

#include <fstream>

const bool InventoryService::LoadItems(const char * file)
{
	std::ifstream fs;
	fs.open(file, std::ifstream::ate | std::ifstream::binary);
	if (!fs.is_open())
		return false;

	int size = fs.tellg();
	fs.seekg(0, std::ios::beg);

	Stream * s = new Stream(size);
	fs.read((char*)s->_raw, size);

	fs.close();

	int itemCount = s->ReadInt32();
	for (size_t i = 0; i < itemCount; i++)
	{
		IItem * nI = new IItem();
		nI->_id = s->ReadInt32();
		nI->_name = s->ReadASCIIString();
		nI->_type = (ItemType)s->ReadInt32();
		nI->_itemLevel = s->ReadInt32();
		nI->_requiredLevel = s->ReadInt32();
		nI->_maxStack = s->ReadInt32();
		nI->_tradable = s->ReadByte();
		nI->_rareGrade = (ItemRareGrade)s->ReadInt32();
		nI->_sellable = s->ReadByte();
		nI->_sellPrice = s->ReadInt32();
		nI->_static = s->ReadByte();
		nI->_warehouseStoreable = s->ReadByte();
		nI->_usable = s->ReadByte();
		nI->_useOnlyByGender = s->ReadByte();
		nI->_bankStorable = s->ReadByte();
		nI->_guildBankStorable = s->ReadByte();

		nI->_useOnlyByClass = s->ReadByte();
		for (byte i = 0; i < nI->_useOnlyByClass; i++)
			nI->_requiredClassList.push_back(s->ReadInt32());

		for (byte i = 0; i < 4; i++)
			nI->_crystals[i] = s->ReadInt32();

		nI->_hasBonus = s->ReadByte();
		for (byte i = 0; i < nI->_hasBonus; i++)
			nI->_bonusList.push_back(s->ReadInt32());

		nI->_buyPrice = s->ReadInt32();
		nI->_coolTime = s->ReadInt32();
		nI->_enchantEnabled = s->ReadByte();
		nI->_itemGrade = (ItemGrade)s->ReadInt32();
		nI->_enchantLevel = s->ReadInt32();
		nI->_masterworkGrade = s->ReadByte();
		nI->_attackModifier = s->ReadInt32();
		nI->_defenseModifier = s->ReadInt32();
		nI->_gender = s->ReadByte();

		nI->_hasEffect = s->ReadByte();
		for (byte i = 0; i < nI->_hasEffect; i++)
			nI->_effectList.push_back(s->ReadInt32());

		_items.push_back(nI);
	}

	return true;
}

Player::PlayerWarehouse * InventoryService::GetStartWarehouse(PlayerClass pClass)
{
	Player::PlayerWarehouse * out = new Player::PlayerWarehouse;
	ZeroMemory(out, sizeof Player::PlayerWarehouse);

	if (pClass == PlayerClass::WARRIOR || pClass == PlayerClass::ARCHER || pClass == PlayerClass::SLAYER || pClass == PlayerClass::REAPER)
	{
		out->armor = 15004;
		out->boots = 15006;
		out->gloves = 15005;
		out->weapon = 0;
	}
	else if (pClass == PlayerClass::BERSERKER || pClass == PlayerClass::LANCER || pClass == PlayerClass::GUNNER || pClass == PlayerClass::BRAWLER)
	{
		out->armor = 15001;
		out->boots = 15003;
		out->gloves = 15002;
		out->weapon = 0;
	}
	else if (pClass == PlayerClass::SORCERER || pClass == PlayerClass::PRIEST || pClass == PlayerClass::MYSTIC || pClass == PlayerClass::NINJA)
	{
		out->armor = 15007;
		out->boots = 15009;
		out->gloves = 15008;
		out->weapon = 0;
	}

	switch (pClass)
	{
	case PlayerClass::WARRIOR:
		out->weapon = 10001;
		break;
	case PlayerClass::BERSERKER:
		out->weapon = 10004;
		break;
	case PlayerClass::BRAWLER:
		out->weapon = 82005;
		break;
	case PlayerClass::GUNNER:
		out->weapon = 55005;
		break;
	case PlayerClass::PRIEST:
		out->weapon = 10007;
		break;
	case PlayerClass::MYSTIC:
		out->weapon = 10008;
		break;
	case PlayerClass::NINJA:
		out->weapon = 58171;
		break;
	case PlayerClass::LANCER:
		out->weapon = 10002;
		break;
	case PlayerClass::SLAYER:
		out->weapon = 10003;
		break;
	case PlayerClass::SORCERER:
		out->weapon = 10005;
		break;
	case PlayerClass::ARCHER:
		out->weapon = 10006;
		break;
	case PlayerClass::REAPER:
		out->weapon = 80396;
		break;
	}

	return out;
}


IItem * InventoryService::ResolveItem(int id)
{
	IItem* out = nullptr;

	for (size_t i = 0; i < _items.size(); i++)
	{
		if (_items[i] && _items[i]->_id == id)
		{
			out = _items[i];
			break;
		}
	}

	return out;
}

void InventoryService::Release()
{
	for (size_t i = 0; i < _items.size(); i++)
	{
		if (_items[i])
		{
			delete _items[i];
			_items[i] = 0;
		}
	}
	_items.clear();
}

void InventoryService::ResolveInventory(std::vector<int>& ids, std::vector<IItem*>& out_items)
{
	for (size_t i = 0; i < ids.size(); i++)
	{
		IItem * j = 0;
		if (j = ResolveItem(ids[i]))
		{
			out_items.push_back(j->_static ? j : j->Clone());
		}
	}

}

std::vector<IItem*> InventoryService::_items;
