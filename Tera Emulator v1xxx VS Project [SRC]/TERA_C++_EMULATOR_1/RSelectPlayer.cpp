#include "RSelectPlayer.h"
#include "Client.hpp"
#include "Account.hpp"
#include "Player.hpp"
#include "PlayerService.h"
#include "SkillService.h"
#include "InventoryService.h"
#include "Server.hpp"
#include "GuildSystem.h"
#include "WorldSystem.h"
#include "CraftingSystem.h"
#include "UnionSystem.h"
#include "Area.h"

RSelectPlayer::RSelectPlayer() : SendPacket(C_SELECT_USER)
{
}

void RSelectPlayer::Process(OpCode opCode, Stream * data, Client * caller)
{
	std::cout << ">" << ServerUtils::HexString(data->_raw, data->_size) << std::endl;//------------DEBUGs
	int lobbyId = data->ReadInt32();
	Player * p = caller->_account->GetPlayer(lobbyId);
	bool result = !p ? false : true;
	caller->_account->_selectedPlayer = p;

	data->Clear();
	data->WriteInt16(16);
	data->WriteInt16(S_SELECT_USER);
	data->WriteInt32(result == true ? 1 : 0);
	data->WriteInt64(0);
	caller->Send(data);
	data->Clear();
	if (!result)
		return;


	//data->WriteHexString("0B00199402000000000000"); //S_UPDATE_CONTENTS_ON_OFF [2]
	//caller->Send(data);
	//data->Clear();

	//data->WriteHexString("0B00199403000000000000"); //S_UPDATE_CONTENTS_ON_OFF [3]
	//caller->Send(data);
	//data->Clear();

	//data->WriteHexString("0B00199404000000000000"); //S_UPDATE_CONTENTS_ON_OFF [4]
	//caller->Send(data);
	//data->Clear();

	//data->WriteHexString("0B00199408000000000000"); //S_UPDATE_CONTENTS_ON_OFF [8]
	//caller->Send(data);
	//data->Clear();

	//data->WriteHexString("0B00199409000000000000"); //S_UPDATE_CONTENTS_ON_OFF [9]
	//caller->Send(data);
	//data->Clear();

	GuildSystem::BrodcastGuildFlag(caller);
	UnionSystem::SendElectionState(caller);
#pragma region S_LOGIN

	data->WriteInt16(0);
	data->WriteInt16(S_LOGIN);

	short name_pos = data->_pos;
	data->WriteInt16(0); // name offset
	short details1_pos = data->_pos;
	data->WriteInt16(0);// details1 offset
	data->WriteInt16(32); //details1 count 
	short details2_pos = data->_pos;
	data->WriteInt16(0); // details2 offset
	data->WriteInt16(64); //details2 count


	data->WriteInt32(p->_model);

	//data->WriteInt16(6516);    //unk 0x7419
	//data->WriteInt16(4012);    //unk 0xac0f
	//data->WriteInt16(32768);   //unk 0x0080
	//data->WriteInt16(256);     //unk 0x0001

	data->WriteInt64(p->_entityId);

	data->WriteInt32(SERVER_ID); //server id
	data->WriteInt32((int)p->_lobbyId);

	
	data->WriteInt32(0); 
	data->WriteByte(1);
	data->WriteInt32(0);
	data->WriteInt32(50);
	data->WriteInt32(110);
	data->Write(p->_data, 8); //int64 data [appearance]

	data->WriteInt16(1);
	data->WriteInt16(p->_level);

	data->WriteInt32(130); //mining
	data->WriteInt16(130);	//plants 
	data->WriteInt16(130);	 //energy

	data->WriteInt32(1);
	data->WriteInt16(0);

	data->WriteInt32(0);


	data->WriteInt64(p->_exp);
	data->WriteInt64(p->_exp); //player exp
	data->WriteInt64(10000); //next level exp

	data->WriteInt32(0);	  //unk items ??
	data->WriteInt32(0);	  //unk items ??
	data->WriteInt32(0);	  //unk items ??
	data->WriteInt32(0); 	  //unk items ??
							  //unk items ??
	data->WriteInt32(59778717); //rested exp1
	data->WriteInt32(59778717); //rested exp2
	data->WriteInt32(1065353216);	 //unk exp ??
	data->WriteInt32(0);	 //----------------------------------------------------------to delete-------------------------------


	data->WriteInt32(p->_playerWarehouse->weapon);
	data->WriteInt32(p->_playerWarehouse->armor);
	data->WriteInt32(p->_playerWarehouse->gloves);
	data->WriteInt32(p->_playerWarehouse->boots);

	data->WriteInt32(p->_playerWarehouse->innerWare);
	data->WriteInt32(p->_playerWarehouse->skin1); //head
	data->WriteInt32(p->_playerWarehouse->skin2); //face

	data->WriteInt64(p->_creationTimeUTC); //unk16
	data->WriteInt32(1); //title

	data->WriteInt64(191429903); //unk16 // 0FFD680B

	data->WriteInt32(1); //weaponModel
	data->WriteInt32(0); //chestModel
	data->WriteInt32(0); //glovesModel
	data->WriteInt32(0); //bootsModel
	data->WriteInt32(0); //unk19
	data->WriteInt32(0); //unk20

	data->WriteInt32(0); //unk21
	data->WriteInt32(0);
	data->WriteInt32(0);
	data->WriteInt32(0);
	data->WriteInt32(0);
	data->WriteInt32(0);

	data->WriteInt32(0);
	data->WriteInt32(0);
	data->WriteInt32(0);

	data->WriteInt16(0);
	data->WriteInt16(0);
	data->WriteInt16(0);
	data->WriteInt16(0);
	data->WriteInt16(0);
	data->WriteInt16(0);

	data->WriteInt32(0);
	data->WriteInt32(0);
	data->WriteInt32(0);
	data->WriteInt16(0);

	data->WriteByte(0);

	data->WriteInt32(0);  //armor skin
	data->WriteInt32(0); 	//skins ??
	data->WriteInt32(0); 	//skins ??
	data->WriteInt32(0); 	//skins ??
	data->WriteInt32(0);	//skins ??
							//skins ??
	data->WriteInt32(1);
	data->WriteInt32(0); //unk 30

	data->WriteInt16(25600);

	data->WriteInt32(0);
	data->WriteByte(0);


	data->WritePos(name_pos);
	data->WriteString(p->_name);

	data->WritePos(details1_pos);
	data->Write(p->_details1, 32);
	data->WritePos(details2_pos);
	data->Write(p->_details2, 64);

	data->WriteInt16(0);//unk 0 0


	data->WritePos(0);
	caller->Send(data);
	data->Clear();
#pragma endregion

#pragma region S_SKILL_LIST
	//data->WriteInt16(0);
	//data->WriteInt16(S_SKILL_LIST);

	//data->WriteInt16(p->_skillList.size());
	//data->WriteInt16(8);
	//data->WriteInt16(8);
	//Skill* sk = nullptr;
	//for (size_t i = 0; i < p->_skillList.size(); i++)
	//{
	//	sk = p->_skillList[i];

	//	data->WriteInt16(sk->unk1);
	//	data->WriteInt32(sk->_skillId);
	//	data->WriteInt16(sk->unk2);
	//	data->WriteInt16(sk->unk3);
	//}

	////data->WriteHexString("090008000800120074270000010012001C00844E000001001C002600A49C0000010026003000C4A28900010030003A00D4C9890001003A00440095A69903010044004E001127000000004E0058002C4C00000000580000002D4C00000000");

	//data->WritePos(0);
	//caller->Send(data);
	//data->Clear();
#pragma endregion



#pragma region S_ARTISAN_SKILL_LIST
	data->WriteInt16(0);
	data->WriteInt16(S_ARTISAN_SKILL_LIST);
	data->WriteInt32(0); //count
	data->WriteInt16(0);
	data->WritePos(0);
	caller->Send(data);
	data->Clear();
#pragma endregion


	data->WriteInt16(0);
	data->WriteInt16(S_NPCGUILD_LIST);
	data->WriteInt16(0); //npc guild count [faction]
	short unk_pos = data->_pos;
	data->WriteInt64(p->_entityId);
	data->WritePos(unk_pos);
	data->WriteInt32(0); //region
	data->WriteInt32(0); //faction
	data->WriteInt32(0); //rank  # enum { suspicious = 0, apprehensive = 3, wavering, neutral, favorable, friendly, trusted, revered }
	data->WriteInt32(0); //reputation
	data->WriteInt32(0); //credits
	data->WritePos(0);
	caller->Send(data);
	data->Clear();

	//data->WriteHexString("520049C00500160001000000000000003C000000000016002200000000000000000022002E00FFFFFFFF000000002E003A00FFFFFFFF000000003A004600FFFFFFFF0000000046000000FFFFFFFF00000000");
	//owner->Send(data->_raw, data->_size); // S_PET_INCUBATOR_INFO_CHANGE
	//data->Clear();

	data->WriteInt16(4);
	data->WriteInt16(S_PET_INFO_CLEAR);
	caller->Send(data);
	data->Clear();

	data->WriteInt16(12);
	data->WriteInt16(S_VIRTUAL_LATENCY);
	data->WriteInt32(0);
	data->WriteInt32(0);
	caller->Send(data);
	data->Clear();

	data->WriteInt16(8);
	data->WriteInt16(S_MOVE_DISTANCE_DELTA);
	data->WriteFloat(200);
	caller->Send(data);
	data->Clear();


	data->WriteInt16(21);
	data->WriteInt16(S_LOAD_TOPO);
	data->WriteInt32(caller->_account->_selectedPlayer->_position->_areaId);
	data->WriteFloat(caller->_account->_selectedPlayer->_position->_X);
	data->WriteFloat(caller->_account->_selectedPlayer->_position->_Y);
	data->WriteFloat(caller->_account->_selectedPlayer->_position->_Z);
	data->WriteByte(0);
	caller->Send(data);
	data->Clear();

	data->WriteInt16(0);
	data->WriteInt16(S_LOAD_HINT);
	data->WriteInt32(0); //unk
	data->WritePos(0);
	caller->Send(data);
	data->Clear();

	data->WriteInt16(16);
	data->WriteInt16(S_SEND_USER_PLAY_TIME);
	data->WriteInt32(p->_entityId);
	data->WriteInt64(3790); //player online time
	caller->Send(data);
	data->Clear();

	data->WriteInt16(44);
	data->WriteInt16(S_LOAD_EP_INFO);
	data->WriteInt32(0); //unk
	data->WriteInt32(0); //unk
	data->WriteInt32(0); //unk
	data->WriteInt32(0); //unk
	data->WriteInt32(0); //unk
	data->WriteInt32(0); //unk
	data->WriteInt32(0); //unk
	data->WriteInt32(0); //unk
	data->WriteInt32(0); //unk
	data->WriteInt32(0); //unk
	caller->Send(data);
	data->Clear();
}


//A601CCA9 32014601 20006601 40005D2B
//0000D755 AC0F0080 0001AC0F 00000150
//37000000 00000100 00000036 00000078
//00000065 00020202 01010001 00010000
//00000000 00000001 00000000 00000000
//00010000 00000000 00010000 00000000
//00480300 00000000 00000000 00000000
//00000000 00000000 00000000 00A30100
//00000000 00000000 00112700 009C3A00
//009D3A00 009E3A00 00000000 00000000
//00000000 005D92C2 0B000000 00010000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000001 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 01000000 00000000
//00640000 00000080 3F010000 00000000
//00005400 61007200 74006100 72006900
//6E006100 00001004 080D0000 0000101B
//1F001010 00001C09 00130C0A 10000D1F
//0C091310 00000113 10131310 1313130F
//0F0F0F0F 0F0F1013 0A000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 0000

//BF02CEBE 04003B00 7263AC0F 00800001
//00000000 00000000 00010028 00000001
//00000001 00000000 00000000 00000000
//00000000 00000000 0000003B 00DC0000
//000000DA 00112700 00D3B581 31000000
//00015037 00000000 00010000 00000000
//00010000 00000000 001E0000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000005 00000005 00000005
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//FFFFFFFF 00000000 00000000 DC007D01
//00000000 7B019C3A 0000D4B5 81310000
//00000150 37000000 00000300 00000000
//00000100 00000000 00001E00 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 01000000 01000000
//01000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00FFFFFF FF000000 00000000 007D011E
//02000000 001C029D 3A0000D5 B5813100
//00000001 50370000 00000004 00000000
//00000001 00000000 0000001E 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00010000 00010000
//00010000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//0000FFFF FFFF0000 00000000 00001E02
//00000000 0000BD02 9E3A0000 D6B58131
//00000000 01503700 00000000 05000000
//00000000 01000000 00000000 1E000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//00000000 00000000 00000100 00000100
//00000100 00000000 00000000 00000000
//00000000 00000000 00000000 00000000
//000000FF FFFFFF00 00000000 000000


