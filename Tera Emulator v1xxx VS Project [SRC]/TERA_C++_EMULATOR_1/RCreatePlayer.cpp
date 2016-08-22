#include "RCreatePlayer.h"

RCreatePlayer::RCreatePlayer() : SendPacket(C_CREATE_USER)
{
}

void RCreatePlayer::Process(OpCode opCode, Stream * stream, Client * caller)
{

	bool result = caller->_account->_playerCount >= MAX_PLAYERS_PER_ACCOUNT;

	if (!result)
	{
		short nameOffset = stream->ReadInt16();
		short details1Offset = stream->ReadInt16();
		short details1Length = stream->ReadInt16();
		short details2Offset = stream->ReadInt16();
		short details2Length = stream->ReadInt16();

		short gender = stream->ReadInt32();
		short race = stream->ReadInt32();
		short pClass = stream->ReadInt32();


		byte * data = new byte[8];
		stream->Read(data, 8);

		stream->_pos = nameOffset - 4;
		std::string name = stream->ReadReceivedString();

		byte* details1 = new byte[32];
		stream->_pos = details1Offset - 4;
		stream->Read(details1, details1Length);

		byte* details2 = new byte[64];
		memset((void*)details2, 0, 64);
		stream->_pos = details2Offset - 4;
		stream->Read(details2, details2Length);


		Player::PlayerInfo * inf = new Player::PlayerInfo;
		Player::PlayerSkinWareHouse * skinW = new Player::PlayerSkinWareHouse;
		Player::PlayerWarehouse * pW = InventoryService::GetStartWarehouse((PlayerClass)pClass);

		Player * newPlayer = new Player();
		newPlayer->_newPlayer = true;
		newPlayer->_name = name;
		newPlayer->_level = 1; //todo add leveling system-> start_level
		newPlayer->_exp = 1;
		newPlayer->_restedExp = 0;
		newPlayer->_maxExp = LevelingService::GetLevelExperience(1);
		newPlayer->_lastOnlineUTC = ServerTimer::GetCurrentUTC();
		newPlayer->_banTimeUTC = 0;


		newPlayer->_details1 = details1;
		newPlayer->_details2 = details2;
		newPlayer->_data = data;

		newPlayer->_playerInfo = inf;
		newPlayer->_playerWarehouse = pW;
		newPlayer->_playerSkinWarehouse = skinW;
		WorldSystem::NewPlayerPosition(newPlayer);


		inf->pClass = (PlayerClass)pClass;
		inf->pGender = (PlayerGender)gender;
		inf->pRace = (PlayerRace)race;


		PlayerService::BuildPlayerStats(newPlayer);

		ZeroMemory(skinW, sizeof Player::PlayerSkinWareHouse);
		caller->_account->AddPlayer(newPlayer);
	}

	stream->Clear();
	stream->WriteInt16(5);
	stream->WriteInt16(S_CREATE_USER);
	stream->WriteByte(result == true ? 0 : 1);
	
	caller->Send(stream);
	stream->Clear();
}
//94001363 28003400 20005400 40000100
//00000300 00000000 00006502 01090E00
//01010064 00000000 41006600 61007200
//61000000 03101003 0A000015 1A101000
//11031000 08100A10 0C171010 10161608
//15100B10 01131013 13101313 130F0F0F
//0F0F0F0F 10130A00 10101000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000