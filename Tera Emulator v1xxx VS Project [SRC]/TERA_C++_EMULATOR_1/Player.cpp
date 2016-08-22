#include "Player.hpp"
#include "ServerTimer.h"

Player::Player() : Entity(), _currentVisitedSection(AreaSection(0, 0, 0))
{
	_toDelete = false;
	_playerSkinWarehouse = 0;
	_playerInfo = 0;
	_playerWarehouse = 0;
	_exp = 0;
	_restedExp = 0;
	_data = 0;
	_details1 = 0;
	_details2 = 0;
	_depositSlots = 0;
	_name = "";
	_lastOnlineUTC = 0;
	_banTimeUTC = 0;
	_creationTimeUTC = 0;
	_newPlayer = false;
	_visibleRange = 2000;
	_stats = 0;
	_userSettings = 0;
}

Player::~Player()
{
	if (_playerSkinWarehouse)
	{
		delete _playerSkinWarehouse;
		_playerSkinWarehouse = 0;
	}
	if (_playerWarehouse)
	{
		delete _playerWarehouse;
		_playerWarehouse = 0;
	}
	if (_playerInfo)
	{
		delete _playerInfo;
		_playerInfo = 0;
	}

	if (_data)
	{
		delete[] _data;
		_data = 0;
	}
	if (_details1)
	{
		delete[] _details1;
		_details1 = 0;
	}
	if (_details2)
	{
		delete[] _details2;
		_details2 = 0;
	}
	if (_position)
	{
		delete _position;
		_position = 0;
	}
	if (_stats)
	{
		delete _stats;
		_stats = 0;
	}
	if (_userSettings)
	{
		delete[] _userSettings;
		_userSettings = 0;
	}

}

const bool Player::AreaSection::operator==(AreaSection& a)
{
	return (continentId == a.continentId) && (areaId == a.areaId) && (sectionId == a.sectionId);
}

const bool Player::AreaSection::operator!=(AreaSection & a)
{
	return (a.areaId != areaId) && (a.sectionId != sectionId);
}

Player::AreaSection::AreaSection(int a, int b, int c)
{
	continentId = a;
	areaId = b;
	sectionId = c;
}
