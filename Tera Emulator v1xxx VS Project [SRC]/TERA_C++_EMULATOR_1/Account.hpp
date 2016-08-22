#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <vector>
#include <string>

#include "Crypt\data_type.hpp"

#define MAX_PLAYERS_PER_ACCOUNT 20

class Player;
class Account
{
public:
	Account();
	~Account();

	unsigned int GetPlayerCount();
	Player* GetPlayer(int id);
	Player* GetPlayer(byte lobbyid);
	void AddPlayer(Player* p);

	std::vector<Player*> _playerList;
	Player * _selectedPlayer;
	std::string _username;
	std::string _password;
	std::string _email;
	int _lasOnlineUtc;
	bool _isGm;
	int _id;
	short _coins;
	int _playerCount;
	short _visibleRange;
	short _remainingPlayTime;
	byte* _ticket;
	byte _lastLobbyId;
	int _ticketSize;
	byte * _accountSettings;
	unsigned short _accountSettingsSize;
	std::string _hardwareInfo;
	int _channel;
	//todo:expand
};
#endif

