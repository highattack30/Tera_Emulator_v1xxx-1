#ifndef PLAYER_H
#define PLAYER_H

#include "Crypt\data_type.hpp"
#include "PlayerEnums.h"
#include "Entity.h"
#include "Area.h"
#include "WorldPosition.h"
#include <vector>
#include "CreatureStats.h"

class IItem; class Skill; class Area;
class Player :public Entity
{
public:
	struct AreaSection
	{
		int continentId; //??
		int areaId; //??
		int sectionId;

		const bool operator ==(AreaSection& a);
		const bool operator !=(AreaSection& a);

		AreaSection(int a, int b, int c);
	};
	struct PlayerWarehouse
	{
		int weapon;
		int armor;
		int gloves;
		int boots;
		int ring1;
		int ring2;
		int earring1;
		int earring2;
		int necklace;
		int innerWare;
		int broch;
		int belt;
		int skin1;
		int skin2;
	};
	struct PlayerSkinWareHouse
	{
		int headSkin;
		int faceSkin;
		int bodySkin;
		int weaponSkin;
		int backSkin;

	};
	struct PlayerInfo
	{
		PlayerClass		pClass;
		PlayerGender	pGender;
		PlayerRace		pRace;
	};

	
public:
	Player();
	~Player();

	PlayerWarehouse * _playerWarehouse;
	PlayerSkinWareHouse* _playerSkinWarehouse;
	PlayerInfo * _playerInfo;

	std::string _name;
	int	_exp;
	int _model;
	int  _restedExp;
	int _maxExp;
	int _lobbyId;
	bool _toDelete;
	byte * _data;
	byte * _details1;
	byte * _details2;
	bool _newPlayer;
	short _level;
	int64_t _lastOnlineUTC;
	int64_t _creationTimeUTC;
	int64_t _banTimeUTC;
	short _depositSlots;
	std::vector<IItem*> _inventoryItems;
	std::vector<IItem*> _deposititems;
	std::vector<Skill*> _skillList;
	
	int _visibleRange;
	byte* _userSettings;
	unsigned int _settingSize;
	CreatureStats* _stats;
	WorldPosition * _position;

	Area * _currentArea;
	std::vector<AreaSection> _visitedSections;
	AreaSection _currentVisitedSection;
};

#endif

//if (Sharun->Players.Lists[i] && packet_l->sender) is_spawP_b = Sharun->Players.Lists[i]->Spawn->in(packet_l->sender);
//if (packet_l
//	&& (!(packet_l->BC_Type & BC_TYPE_NOT_ME) || ((packet_l->BC_Type & BC_TYPE_NOT_ME) && packet_l->sender->Players_ID != i))
//	&& Sharun->Players.Lists[i]
//	&& Sharun->Players.Lists[i]->loged
//	&& Sharun->Players.Lists[i]->DjW
//	&& Sharun->Players.Lists[i]->DjW->Zone
//	&& ((packet_l->BC_Type & BC_TYPE_MAIN)
//		|| ((packet_l->BC_Type & BC_TYPE_ZONE) && packet_l->sender && packet_l->sender->DjW->Zone == Sharun->Players.Lists[i]->DjW->Zone)
//		|| ((packet_l->BC_Type & BC_TYPE_ARROUND) && packet_l->sender && packet_l->sender->DjW->Zone == Sharun->Players.Lists[i]->DjW->Zone
//			&& ((packet_l->Op_ID == S_DESPAWN_NPC && ((packet_l->BC_Type & BC_TYPE_FORCE) || getDistance_2D(packet_l->sender->DjW->XYZ, Sharun->Players.Lists[i]->DjW->XYZ) > 100) && is_spawP_b)
//				|| (packet_l->Op_ID == S_SPAWN_USER  && getDistance_2D(packet_l->sender->DjW->XYZ, Sharun->Players.Lists[i]->DjW->XYZ) <= 100 && !is_spawP_b)
//				/*					  || ((packet_l->Op_ID == S_USER_EXTERNAL_CHANGE || packet_l->Op_ID == S_MOUNT_VEHICLE || packet_l->Op_ID == S_UNMOUNT_VEHICLE) && getDistance_2D(packet_l->sender->DjW->XYZ, Sharun->Players.Lists[i]->DjW->XYZ) <= 100
//				&& (Sharun->Players.Lists[i] == packet_l->sender || (Sharun->Players.Lists[i] != packet_l->sender && is_spawP_b)))
//				*/ || (packet_l->Op_ID == S_CHAT && getDistance_2D(packet_l->sender->DjW->XYZ, Sharun->Players.Lists[i]->DjW->XYZ) < 200)
//				|| ((packet_l->Op_ID != S_DESPAWN_NPC && packet_l->Op_ID != S_SPAWN_USER && packet_l->Op_ID != S_CHAT) && getDistance_2D(packet_l->sender->DjW->XYZ, Sharun->Players.Lists[i]->DjW->XYZ) <= 100
//					&& (Sharun->Players.Lists[i] == packet_l->sender || (Sharun->Players.Lists[i] != packet_l->sender && is_spawP_b)))
//				)
//			)
//		)
//	) {