#include "PlayerService.h"

#include "Area.h"
#include "Stream.h"
#include "Account.hpp"
#include "Client.hpp"
#include "Player.hpp"
#include "mysql\MySqlDriver.h"
#include "mysql\SQLStream.h"
#include "ServerUtils.hpp"
#include "OpCodesEnum.h"

#include "PlayerEnums.h"
#include "Player.hpp"
#include "Account.hpp"
#include "WorldPosition.h"
#include "InventoryService.h"
#include "WorldSystem.h"
#include "SkillService.h"
#include "ServerTimer.h"
#include "LevelingService.h"
#include "BroadcastService.h"

#include <sstream>
#include <Windows.h>


const int PlayerService::InitializePlayerService(MySqlDriver * driver)
{
	int t = 0;
	while (!driver->Lock())
	{
		continue;
	}
	int count = 0;

	sql::ResultSet * result = 0;
	result = driver->ExecuteQuery("SELECT * FROM accounts");
	std::stringstream sbuild;
	while (result->next())
	{
		Account * newAccount = new Account();

		newAccount->_id = count + 1;

		newAccount->_username = result->getString(2).c_str();
		newAccount->_password = result->getString(3).c_str();
		newAccount->_email = result->getString(4).c_str();
		newAccount->_lasOnlineUtc = result->getInt64(5);
		newAccount->_coins = result->getInt(6);
		newAccount->_isGm = result->getBoolean(7);
		newAccount->_remainingPlayTime = (short)result->getInt(8);
		newAccount->_hardwareInfo = result->getString(9).c_str();

		std::istream * accountSettings = result->getBlob(10);
		newAccount->_accountSettingsSize = result->getInt(11);

		newAccount->_accountSettings = new byte[newAccount->_accountSettingsSize];
		accountSettings->read((char*)newAccount->_accountSettings, newAccount->_accountSettingsSize);
		ServerUtils::UndoEscapedData(newAccount->_accountSettings, newAccount->_accountSettingsSize);

		delete accountSettings;
		accountSettings = 0;

		_accountsNames.push_back(newAccount->_username);

		sbuild << "SELECT * FROM players WHERE username='" << newAccount->_username << "'";
		sql::ResultSet* res = 0; res = driver->ExecuteQuery(sbuild.str().c_str());
		while (res->next()) //Get players
		{
			Player * newPlayer = new Player();

			newPlayer->_newPlayer = false;
			newPlayer->_playerInfo = new Player::PlayerInfo;
			newPlayer->_playerSkinWarehouse = new Player::PlayerSkinWareHouse;
			newPlayer->_playerWarehouse = new Player::PlayerWarehouse;
			ZeroMemory(newPlayer->_playerInfo, sizeof Player::PlayerInfo);
			ZeroMemory(newPlayer->_playerWarehouse, sizeof Player::PlayerWarehouse);
			ZeroMemory(newPlayer->_playerSkinWarehouse, sizeof Player::PlayerSkinWareHouse);


			newPlayer->_name = res->getString(2).c_str();
			_playerNames.push_back(newPlayer->_name);
			newPlayer->_position = new WorldPosition();
			newPlayer->_position->_X = (float)res->getDouble(3);
			newPlayer->_position->_Y = (float)res->getDouble(4);
			newPlayer->_position->_Z = (float)res->getDouble(5);
			newPlayer->_position->_heading = (float)res->getInt(6);
			newPlayer->_position->_areaId = (short)res->getInt(12);

			newPlayer->_playerInfo->pRace = (PlayerRace)res->getInt(7);
			newPlayer->_playerInfo->pGender = (PlayerGender)res->getInt(8);
			newPlayer->_playerInfo->pClass = (PlayerClass)res->getInt(9);

			newPlayer->_exp = res->getInt(10);
			newPlayer->_restedExp = res->getInt(11);
			newPlayer->_level = (short)res->getInt(13);

			newPlayer->_lastOnlineUTC = res->getInt64(17);
			newPlayer->_creationTimeUTC = res->getInt64(18);
			newPlayer->_banTimeUTC = res->getInt64(19);

			std::istream *details1S = res->getBlob(14);
			std::istream *details2S = res->getBlob(15);
			std::istream * dataS = res->getBlob(16);

			newPlayer->_data = new byte[6];
			newPlayer->_details1 = new byte[32];
			newPlayer->_details2 = new byte[64];
			details1S->read((char*)newPlayer->_details1, 32);
			details2S->read((char*)newPlayer->_details2, 64);
			dataS->read((char*)newPlayer->_data, 6);
			ServerUtils::UndoEscapedData(newPlayer->_data, 8);
			ServerUtils::UndoEscapedData(newPlayer->_details1, 32);
			ServerUtils::UndoEscapedData(newPlayer->_details2, 64);

			delete dataS;
			dataS = 0;
			delete details1S;
			details1S = 0;
			delete details2S;
			details2S = 0;

			sbuild.clear(); sbuild.str("");
			sbuild << "SELECT * FROM `player_warehouse` WHERE username='" << newAccount->_username << "' AND name='" << newPlayer->_name << "'";
			sql::ResultSet *wareHouseaResult = 0; wareHouseaResult = driver->ExecuteQuery(sbuild.str().c_str());
			sbuild.clear(); sbuild.str("");
			sbuild << "SELECT * FROM `player_skin_warehouse` WHERE username='" << newAccount->_username << "' AND name='" << newPlayer->_name << "'";
			sql::ResultSet * skinWareHouseaResult = 0; skinWareHouseaResult = driver->ExecuteQuery(sbuild.str().c_str());
			sbuild.clear(); sbuild.str("");
			sbuild << "SELECT * FROM `player_inventory` WHERE username='" << newAccount->_username << "' AND name='" << newPlayer->_name << "'";
			sql::ResultSet * inventory = 0; inventory = driver->ExecuteQuery(sbuild.str().c_str());
			sbuild.clear(); sbuild.str("");
			sbuild << "SELECT * FROM `player_deposit` WHERE username='" << newAccount->_username << "' AND name='" << newPlayer->_name << "'";
			sql::ResultSet * deposit = 0; deposit = driver->ExecuteQuery(sbuild.str().c_str());
			sbuild.clear(); sbuild.str("");
			sbuild << "SELECT * FROM `player_skills` WHERE username='" << newAccount->_username << "' AND name='" << newPlayer->_name << "'";
			sql::ResultSet * skills = 0; skills = driver->ExecuteQuery(sbuild.str().c_str());

			sbuild << "SELECT * FROM `player_settings` WHERE username='" << newAccount->_username << "' AND name='" << newPlayer->_name << "'";
			sql::ResultSet * settings = 0; settings = driver->ExecuteQuery(sbuild.str().c_str());

			if (wareHouseaResult && wareHouseaResult->next())
			{
				newPlayer->_playerWarehouse->weapon = wareHouseaResult->getInt(3);
				newPlayer->_playerWarehouse->armor = wareHouseaResult->getInt(4);
				newPlayer->_playerWarehouse->gloves = wareHouseaResult->getInt(5);
				newPlayer->_playerWarehouse->boots = wareHouseaResult->getInt(6);
				newPlayer->_playerWarehouse->ring1 = wareHouseaResult->getInt(7);
				newPlayer->_playerWarehouse->ring2 = wareHouseaResult->getInt(8);
				newPlayer->_playerWarehouse->earring1 = wareHouseaResult->getInt(9);
				newPlayer->_playerWarehouse->earring2 = wareHouseaResult->getInt(10);
				newPlayer->_playerWarehouse->necklace = wareHouseaResult->getInt(11);
				newPlayer->_playerWarehouse->innerWare = wareHouseaResult->getInt(12);
				newPlayer->_playerWarehouse->broch = wareHouseaResult->getInt(13);
				newPlayer->_playerWarehouse->belt = wareHouseaResult->getInt(14);
				newPlayer->_playerWarehouse->skin1 = wareHouseaResult->getInt(15);
				newPlayer->_playerWarehouse->skin2 = wareHouseaResult->getInt(16);


				delete wareHouseaResult;
				wareHouseaResult = 0;
			}

			if (skinWareHouseaResult && skinWareHouseaResult->next())
			{
				newPlayer->_playerSkinWarehouse->headSkin = skinWareHouseaResult->getInt(3);
				newPlayer->_playerSkinWarehouse->faceSkin = skinWareHouseaResult->getInt(4);
				newPlayer->_playerSkinWarehouse->bodySkin = skinWareHouseaResult->getInt(5);
				newPlayer->_playerSkinWarehouse->weaponSkin = skinWareHouseaResult->getInt(6);
				newPlayer->_playerSkinWarehouse->backSkin = skinWareHouseaResult->getInt(7);

				delete skinWareHouseaResult;
				skinWareHouseaResult = 0;
			}
			std::vector<int> _inventory;
			std::vector<int> _deposit;
			std::vector<int> _skillsList;
			if (inventory && inventory->next())
			{

				int itemsCount = inventory->getInt(4);
				std::istream * blobData = inventory->getBlob(3);

				unsigned char toRead[4];
				for (int i = 0; i < itemsCount; i++)
				{
					blobData->read((char*)toRead, 4);
					_inventory.push_back((toRead[0] << 24) | (toRead[1] << 16) | (toRead[2] << 8) | (toRead[3]));
				}

				delete blobData;
				blobData = 0;

				delete inventory;
				inventory = 0;
			}
			if (deposit && deposit->next())
			{
				int itemsCount = deposit->getInt(4);
				std::istream * blobData = deposit->getBlob(3);

				unsigned char toRead[4];
				for (int i = 0; i < itemsCount; i++)
				{
					blobData->read((char*)toRead, 4);
					_deposit.push_back((toRead[0] << 24) | (toRead[1] << 16) | (toRead[2] << 8) | (toRead[3]));
				}

				delete blobData;
				blobData = 0;
				delete deposit;
				deposit = 0;
			}


			if (skills && skills->next())
			{
				int skillsCount = skills->getInt(4);
				std::istream * blobData = skills->getBlob(3);
				unsigned char toRead[4];
				for (size_t i = 0; i < skillsCount; i++)
				{
					blobData->read((char*)toRead, 4);
					_skillsList.push_back((toRead[3] << 24) | (toRead[2] << 16) | (toRead[1] << 8) | (toRead[0]));
				}
				//we read skills ids from database blob
				delete blobData;
				blobData = 0;
				delete skills;
				skills = 0;
			}

			InventoryService::ResolveInventory(_inventory, newPlayer->_inventoryItems);
			InventoryService::ResolveInventory(_deposit, newPlayer->_deposititems);
			SkillService::ResolveSkillSet(_skillsList, newPlayer->_skillList); //and resolve thouse ids into skills from skill service

			BuildPlayerStats(newPlayer);

			newPlayer->_model = 10000;
			newPlayer->_model += ((ushort)newPlayer->_playerInfo->pRace * 2 + 1 + (ushort)newPlayer->_playerInfo->pGender) * 100;
			newPlayer->_model += (ushort)newPlayer->_playerInfo->pClass + 1; //player model


			newAccount->_playerCount++;
			newAccount->_playerList.push_back(newPlayer);
		}
		if (res)
		{
			delete res;
			res = 0;
		}

		sbuild.clear();
		sbuild.str("");



		sbuild.clear();	sbuild.str("");
		_accounts.push_back(newAccount);
		if (count == -1)
			count = 1;
		else
			count++;
	}



	_driver = driver;
	driver->Unlock();
	return count;
}

const bool PlayerService::CheckPlayerNameForm(std::string name)
{
	return true; //todo
}

const bool PlayerService::CheckPlayerName(std::string name)
{

	for (size_t i = 0; i < _playerNames.size(); i++)
	{
		if (_playerNames[i] == name)
			return false;
	}
	return true;
}

const bool PlayerService::CanCreatePlayer(Account * account)
{
	return (account->_playerCount < MAX_PLAYERS_PER_ACCOUNT);
}

const int PlayerService::PerformAccountLogin(const char * unsername, const char * password)
{
	_mainMutex.lock();

	int result = -1;
	for (size_t i = 0; i < _accountsNames.size(); i++)
	{
		if (_accountsNames[i] == unsername)
		{
			result = i;
			break;
		}
	}
	_mainMutex.unlock();
	return result;
}

Account * PlayerService::GetAccount(int accountIndex)
{

	Account * result = nullptr;
	if (accountIndex >= 0 && accountIndex <= (int)_accounts.size())
	{
		result = _accounts[accountIndex];
	}

	return result;
}

unsigned int PlayerService::GetAccountCount()
{
	return _accounts.size();
}

void PlayerService::CreatePlayer(Account *  account, std::string name, short playerGender, short playerClass, short playerRace, byte* details1, byte* details2, byte* data)
{


}


void PlayerService::SaveAccountData(Account * account)
{

	if (!account || !_driver)
		return;

	while (!_driver->Lock())
		continue;



	sql::Statement * st = _driver->GetConnection()->createStatement();
	SQLStream s = SQLStream();
	s.WriteString("UPDATE accounts SET accountSettings={'");
	s.Write((const char*)account->_accountSettings, account->_accountSettingsSize);
	s.WriteString("'},");
	s.WriteString("accountSettingsSize={");
	s[account->_accountSettingsSize];
	s.WriteString("} WHERE username=");
	s.WriteAndQuoteString(account->_username.c_str());
	s.EscapeString();


	try
	{
		st->execute((const char*)s);
	}
	catch (const sql::SQLException& e)
	{
		std::cout << ">>Error:" << e.what() << "\n";
	}
	s.Clear();

	for (size_t i = 0; i < account->_playerList.size(); i++)
	{
		Player * p = account->_playerList[i];
		if (p->_toDelete && !p->_newPlayer)
		{
			s.Clear();
			s.WriteString("DELETE FROM players WHERE name='"); s._raw += p->_name; s._raw += "'";
			try
			{
				st->execute((const char*)s);
			}
			catch (const sql::SQLException& e)
			{
				std::cout << ">>Error:" << e.what() << "\n";
			}

			s.Clear();
			s.WriteString("DELETE FROM player_warehouse WHERE name='"); s._raw += p->_name; s._raw += "'";
			try
			{
				st->execute((const char*)s);
			}
			catch (const sql::SQLException& e)
			{
				std::cout << ">>Error:" << e.what() << "\n";
			}

			s.Clear();
			s.WriteString("DELETE FROM player_skin_warehouse WHERE name='"); s._raw += p->_name; s._raw += "'";
			try
			{
				st->execute((const char*)s);
			}
			catch (const sql::SQLException& e)
			{
				std::cout << ">>Error:" << e.what() << "\n";
			}

			std::cout << ">> DELETED name[" << p->_name << "]\n\n";

		}
		else if (p->_toDelete && p->_newPlayer)
			continue;
		else if (!p->_toDelete && p->_newPlayer)
		{

			byte *details1 = (byte*)SQLStream::EscapeData((char*)p->_details1, 32);
			byte*details2 = (byte*)SQLStream::EscapeData((char*)p->_details2, 64);
			byte*data = (byte*)SQLStream::EscapeData((char*)p->_data, 8);

			s.Clear();
			s.WriteString("INSERT INTO players(username,name,x,y,z,h,race,gender,class,exp,restedExp,areaId,level,details1,details2,headData,lastOnlineUTC,creationTimeUTC,banTimeUTC) VALUES(");
			s.WriteAndQuoteString(account->_username.c_str());
			s[','];
			s.WriteAndQuoteString(p->_name.c_str());
			s[','];
			s[p->_position->_X];
			s[','];
			s[p->_position->_Y];
			s[','];
			s[p->_position->_Z];
			s[','];
			s[p->_position->_heading];
			s[','];
			s[p->_playerInfo->pRace];
			s[','];
			s[p->_playerInfo->pGender];
			s[','];
			s[p->_playerInfo->pClass];
			s[','];
			s[p->_exp];
			s[','];
			s[p->_restedExp];
			s[','];
			s[1]; //area id
			s[','];
			s[p->_level];
			s[','];

			s['\''];
			s.Write((char*)details1, 32);
			s['\''];

			s[','];

			s['\''];
			s.Write((char*)details2, 64);
			s['\''];

			s[','];

			s['\''];
			s.Write((char*)data, 8);
			s['\''];

			s[','];
			s[p->_lastOnlineUTC];
			s[','];
			s[p->_creationTimeUTC];
			s[','];
			s[p->_banTimeUTC];
			s[')'];
			bool result = 0;
			try
			{
				result = st->execute((const char*)s);
			}
			catch (const sql::SQLException& e)
			{
				std::cout << ">>Error:" << e.what() << "\n";
			}

			details1[31] = 0;
			delete details1;
			details1 = 0;
			details2[63] = 0;
			delete details2;
			details2 = 0;
			data[7] = 0;
			delete data;
			data = 0;


			s.Clear();
			s.WriteString("INSERT INTO player_warehouse(username,name,weapon,chest,gloves,boots,ring1,ring2,earring1,earring2,necklace,innerware,broch,belt,skin1,skin2) VALUES(");
			s.WriteAndQuoteString(account->_username.c_str());
			s[','];
			s.WriteAndQuoteString(p->_name.c_str());
			s[','];
			s[p->_playerWarehouse->weapon];
			s[','];
			s[p->_playerWarehouse->armor];
			s[','];
			s[p->_playerWarehouse->gloves];
			s[','];
			s[p->_playerWarehouse->boots];
			s[','];
			s[p->_playerWarehouse->ring1];
			s[','];
			s[p->_playerWarehouse->ring2];
			s[','];
			s[p->_playerWarehouse->earring1];
			s[','];
			s[p->_playerWarehouse->earring2];
			s[','];
			s[p->_playerWarehouse->necklace];
			s[','];
			s[p->_playerWarehouse->innerWare];
			s[','];
			s[p->_playerWarehouse->broch];
			s[','];
			s[p->_playerWarehouse->belt];
			s[','];
			s[p->_playerWarehouse->skin1];
			s[','];
			s[p->_playerWarehouse->skin2];
			s[')'];

			result = 0;
			try
			{
				result = st->execute((const char*)s);
			}
			catch (const sql::SQLException& e)
			{
				std::cout << ">>Error:" << e.what() << "\n";
			}

			s.Clear();
			s.WriteString("INSERT INTO player_skin_warehouse(username,name,headSkin,faceSkin,bodySkin,weaponSkin,backSkin) VALUES(");
			s.WriteAndQuoteString(account->_username.c_str());
			s[','];
			s.WriteAndQuoteString(p->_name.c_str());
			s[','];
			s[p->_playerSkinWarehouse->headSkin];
			s[','];
			s[p->_playerSkinWarehouse->faceSkin];
			s[','];
			s[p->_playerSkinWarehouse->bodySkin];
			s[','];
			s[p->_playerSkinWarehouse->weaponSkin];
			s[','];
			s[p->_playerSkinWarehouse->backSkin];
			s[')'];
			result = 0;
			try
			{
				result = st->execute((const char*)s);
			}
			catch (const sql::SQLException& e)
			{
				std::cout << ">>Error:" << e.what() << "\n";
			}



			//todo add inventory data , bank data, guild data, skill data etc
		}
	}

	_driver->Unlock();
}


void PlayerService::SendPlayerSettings(Client * client)
{

}

void PlayerService::ReleaseData()
{
	for (unsigned int i = 0; i < _accounts.size(); i++)
	{
		if (_accounts[i])
		{
			SaveAccountData(_accounts[i]);

			delete _accounts[i];
			_accounts[i] = 0;
		}
	}
	_accounts.clear();

}


void PlayerService::BuildPlayerStats(Player * p)
{
	CreatureStats * stats = new CreatureStats();
	ZeroMemory(stats, sizeof CreatureStats);


	stats->_maxHp = 1020;
	stats->_maxMp = 1010;
	stats->_currentHp = 400;
	stats->_currentMp = 400;

	stats->_attack = 1100;
	stats->_defense = 1120;
	stats->_power = 23;
	stats->_endurance = 180;
	stats->_attackSpeed = 123;
	stats->_movementSpeed = 143;
	stats->_balance = 2400;
	stats->_impact = 3400;
	stats->_balanceFactor = 45.25;
	stats->_impactFactor = 78.45;
	stats->_critChance = 180.24;
	stats->_critPower = 2.24;
	stats->_critResist = 3.35;
	stats->_stunResistance = 56.50;

	if (p && p->_stats)
	{
		delete p->_stats;
		p->_stats = 0;
	}
	else if (!p)
	{
		delete stats;
		stats = 0;
		return;
	}

	p->_stats = stats;
}

std::vector<Account*> PlayerService::_accounts;
std::mutex PlayerService::_mainMutex;
std::mutex PlayerService::_dbMutex;
std::vector<std::string> PlayerService::_playerNames;
std::vector<std::string> PlayerService::_accountsNames;
MySqlDriver * PlayerService::_driver = 0;


