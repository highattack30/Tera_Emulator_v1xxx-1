#ifndef PLAYER_SERVICE_H
#define PLAYER_SERVICE_H

#include <thread>
#include <vector>
#include <mutex>

#include "PlayerEnums.h"
#include "Crypt\data_type.hpp"


class Account;
class Player;
class Client;
class MySqlDriver;
class PlayerService
{
public:
	static const int InitializePlayerService(MySqlDriver * driver);

	static const bool CheckPlayerNameForm(std::string name);
	static const bool CheckPlayerName(std::string name);
	static const bool CanCreatePlayer(Account * account);
	static const int PerformAccountLogin(const char* unsername, const char* password);
	static Account * GetAccount(int accountIndex);
	static unsigned int GetAccountCount();
	static void CreatePlayer(Account *  account, std::string name, short playerGender, short playerClass, short playerRace, byte* details1, byte* details2, byte* data);
	static void SaveAccountData(Account * account);
	static void SendAccountSettings(Client* client);
	static void SendPlayerSettings(Client* client);
	static void MovePlayer(Client* client, float x, float y, float z, int heading, float x2, float y2, float z2, int type, short speed, byte unk, int time);
	static void ReleaseData();
	static void SendPlayerStats(Client* client);
	static const bool RequestPlayerDeletion(Client * client ,int id);
	static void BuildPlayerStats(Player * p);
private:
	

	static std::vector<Account*> _accounts;
	static std::vector<std::string> _playerNames;
	static std::vector<std::string> _accountsNames;
	static std::mutex _mainMutex;
	static std::mutex _dbMutex;
	static MySqlDriver * _driver;

};

#endif