#include "RGetPlayerList.h"
#include "Client.hpp"
#include "Account.hpp"
#include "Player.hpp"
#include "PlayerService.h"
#include "ServerTimer.h"

RGetPlayerList::RGetPlayerList() : SendPacket(C_GET_USER_LIST)
{

}

void RGetPlayerList::Process(OpCode opCode, Stream * stream, Client * caller)
{
	stream->WriteInt16(0);
	stream->WriteInt16(S_GET_USER_LIST);


	stream->WriteInt16((short)caller->_account->_playerList.size());		 //player count
													 //stream->WriteInt32(playerCount == 0 ? 0 : 1);  //player array offset
	stream->WriteInt16(35); // character offset

	stream->WriteByte(0);
	stream->WriteInt32(0);
	stream->WriteInt32(((int)MAX_PLAYERS_PER_ACCOUNT)); //unk

	stream->WriteInt32(1);	//unk 
	stream->WriteInt16(0);
	stream->WriteInt32(40);//unk reaper needed level?


	stream->WriteInt32(0);
	stream->WriteInt32(24);	//unk

	int location = 0; int next = 0;
	//size 435 bytes [each player]
	for (size_t i = 0; i < caller->_account->_playerList.size(); i++)
	{
		Player * p = caller->_account->_playerList[i];
		if (!p || p->_toDelete)
			continue;
		p->_lobbyId = i + 1;

		stream->WriteInt16(stream->_pos);
		next = stream->_pos;
		stream->WriteInt16(0);	 //unk
		stream->WriteInt32(0);

		short name_pos = stream->_pos;
		stream->WriteInt16(0);

		short details1_pos = stream->_pos;
		stream->WriteInt16(0);
		stream->WriteInt16(32);

		short details2_pos = stream->_pos;
		stream->WriteInt16(0);
		stream->WriteInt16(64);

		short unk_pos = stream->_pos;
		stream->WriteInt16(0); //unk

		stream->WriteInt32(p->_entityId);

		stream->WriteInt32(p->_playerInfo->pGender);
		stream->WriteInt32(p->_playerInfo->pRace);
		stream->WriteInt32(p->_playerInfo->pClass);
		stream->WriteInt32(p->_level);

		stream->WriteInt32(2007);//unk map id1 ? 
		stream->WriteInt32(1308); //unk 1c050000 1308

		stream->WriteInt32(1); // alive ?


		stream->WriteInt32(1); //area ID

		stream->WriteInt32(1); //section id

							   //start velika

		stream->WriteInt64(ServerTimer::GetCurrentUTC()); //


		stream->WriteByte(p->_banTimeUTC > 0 ? 1 : 0); //deleteion in progress
		stream->WriteInt64(p->_banTimeUTC > 0 ? p->_banTimeUTC : 0); //unk
		stream->WriteInt32(abs((int)(p->_banTimeUTC - ServerTimer::GetCurrentUTC()))); //time until character deletion

		stream->WriteInt32(p->_playerWarehouse->weapon);
		stream->WriteInt32(p->_playerWarehouse->earring1);//earring 1? 
		stream->WriteInt32(p->_playerWarehouse->earring2);//earring 2?
		stream->WriteInt32(p->_playerWarehouse->armor);
		stream->WriteInt32(p->_playerWarehouse->gloves);
		stream->WriteInt32(p->_playerWarehouse->boots);
		stream->WriteInt32(0); //unk
		stream->WriteInt32(p->_playerWarehouse->ring1);//ring 1? 
		stream->WriteInt32(p->_playerWarehouse->ring2);//ring 2?
		stream->WriteInt32(p->_playerWarehouse->innerWare);//innerware? 
		stream->WriteInt32(p->_playerWarehouse->skin1);//head?
		stream->WriteInt32(p->_playerWarehouse->skin2);//face? 

		stream->Write(p->_data, 8);

		stream->WriteInt64(0); //8
		stream->WriteInt32(0); //4
		stream->WriteInt16(0); //2


		stream->WriteInt32(0xA85C54e6); //unk

		for (size_t i = 0; i < 32; i++)
		{
			stream->WriteInt32(0); //unk
		}

		/*
		//stream->WriteInt32(p->_playerSkinWarehouse->headSkin); //mask?
		//stream->WriteInt32(p->_playerSkinWarehouse->faceSkin);// unk
		//stream->WriteInt32(p->_playerSkinWarehouse->backSkin);//weaponSkin
		//stream->WriteInt32(p->_playerSkinWarehouse->weaponSkin);//costume
		//stream->WriteInt32(p->_playerSkinWarehouse->bodySkin);// unk
		//stream->WriteInt32(0);// new
		//
		//stream->WriteByte(0);  // weapon enchanting level
		//stream->WriteByte(0); // armor enchanting level
		//stream->WriteByte(0); // gloves enchanting level
		//stream->WriteByte(0); // boots enchanting level
		//stream->WriteInt32(416294);
		//stream->WriteInt32(2430063);
		//stream->WriteByte(1);
		//stream->WriteByte(0);
		//
		//stream->WriteInt32(0);
		//stream->WriteByte(1); // new
		//
		//stream->WriteInt32(100);
		//stream->WriteInt32(820); // achivements
		//stream->WriteInt32(0);
		//stream->WriteInt32(0);
		//stream->WriteInt32(0);
		*/

		stream->WriteInt32(0xa3010000); //unk 419
		stream->WriteInt32(1); //unk 1
		stream->WriteInt16(0); //unk 0
		stream->WriteInt32(0x40060000); //unk 1600

		stream->WriteByte(0); //unk 0

		stream->WriteInt32(0);  //unk 0
		stream->WriteInt32(0);  //unk 0
		stream->WriteInt32(0);  //unk 0
		stream->WriteInt32(0);  //unk 0


		stream->WritePos(name_pos);
		stream->WriteString(p->_name);

		stream->WritePos(details1_pos);
		stream->Write(p->_details1, 32);

		stream->WritePos(details2_pos);
		stream->Write(p->_details2, 64);

		stream->WritePos(unk_pos); //guild name offset?
		stream->WriteInt16(0);

		if (caller->_account->_playerList.size() > 1)
		{
			stream->WritePos(next);
		}

	}
	stream->WritePos(0);
	caller->Send(stream);
	stream->Clear();

	//FeedbackService::SendAccountPackageList(c);


	stream->WriteHexString("1300B47C0F0000000000000000000000000000");
	caller->Send(stream); //S_CONFIRM_INVITE_CODE_BUTTON
	stream->Clear();

	stream->WriteHexString("0B00199402000000000000"); //S_UPDATE_CONTENTS_ON_OFF [2]
	caller->Send(stream);
	stream->Clear();

	stream->WriteHexString("0B00199403000000000000"); //S_UPDATE_CONTENTS_ON_OFF [3]
	caller->Send(stream);
	stream->Clear();

	stream->WriteHexString("0B00199404000000000000"); //S_UPDATE_CONTENTS_ON_OFF [4]
	caller->Send(stream);
	stream->Clear();

	stream->WriteHexString("0B00199408000000000000"); //S_UPDATE_CONTENTS_ON_OFF [8]
	caller->Send(stream);
	stream->Clear();

	stream->WriteHexString("0B00199409000000000000"); //S_UPDATE_CONTENTS_ON_OFF [9]
	caller->Send(stream);
	stream->Clear();


	stream->WriteInt16(caller->_account->_accountSettingsSize == 0 ? 8 : caller->_account->_accountSettingsSize + 4);
	stream->WriteInt16(S_LOAD_CLIENT_ACCOUNT_SETTING);
	stream->WriteInt32(caller->_account->_accountSettingsSize == 0 ? 8 : caller->_account->_accountSettingsSize);
	if (caller->_account->_accountSettingsSize > 0)
		stream->Write(caller->_account->_accountSettings, caller->_account->_accountSettingsSize);
	stream->WritePos(0);
	caller->Send(stream);
}

//000000005D 01710120
//00910140 00D10101 50370000 00000005
//00000000 00000001 000000D7 0700001C
//05000001 00000019 000000D9 2309001E
//A4A05700 00000000 00000000 00000000
//E6545CA8 11270000 00000000 00000000
//9C3A0000 9D3A0000 9E3A0000 00000000
//00000000 00000000 00000000 00000000
//00000000 65000202 02010100 00000000
//00000000 00000000 0000E654 5CA80000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 0000A301
//00000100 00000000 01640000 00000000
//00000000 00000000 00000000 00540061
//00720074 00610072 0069006E 00610000
//00100408 0D000000 00101B1F 00101000
//001C0900 130C0A10 000D1F0C 09131000
//00011310 13131013 13130F0F 0F0F0F0F
//0F10130A 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00












