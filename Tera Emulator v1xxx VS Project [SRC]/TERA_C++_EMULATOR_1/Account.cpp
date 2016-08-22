#include "Account.hpp"
#include "Player.hpp"


Account::Account()
{
	_isGm = false;
	_lasOnlineUtc = 0;
	_username = "";
	_password = "";
	_email = "";
	_coins = 0;
	_remainingPlayTime = 0;
	_visibleRange = 0;
	_selectedPlayer = 0;
	_accountSettings = 0;
	_accountSettingsSize = 0;
	_lastLobbyId = 0;
}

Account::~Account()
{
	for (size_t i = 0; i < _playerList.size(); i++)
	{
		if (_playerList[i])
		{
			delete _playerList[i];
			_playerList[i] = 0;
		}
	}
	_playerList.clear();

	if (_ticket)
	{
		delete[] _ticket;
		_ticket = 0;
	}

	if (_accountSettings)
	{
		delete[] _accountSettings;
		_accountSettings = 0;
	}

	_selectedPlayer = 0;
}

unsigned int Account::GetPlayerCount()
{
	return _playerList.size();
}

Player * Account::GetPlayer(int id)
{
	Player* out_player = nullptr;
	for (size_t i = 0; i < _playerList.size(); i++)
	{
		if (_playerList[i])
			if (_playerList[i]->_entityId == id)
			{
				out_player = _playerList[i];
				break;
			}
	}

	return out_player;
}

Player * Account::GetPlayer(byte lobbyid)
{
	for (size_t i = 0; i < _playerList.size(); i++)
	{
		if (_playerList[i] && _playerList[i]->_lobbyId == lobbyid)
			return _playerList[i];
	}
	return nullptr;
}

void Account::AddPlayer(Player * p)
{
	_playerList.push_back(p);
	_playerCount++;
}
