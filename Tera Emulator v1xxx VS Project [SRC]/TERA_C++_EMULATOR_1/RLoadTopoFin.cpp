#include "RLoadTopoFin.h"

RLoadTopoFin::RLoadTopoFin() : SendPacket(C_LOAD_TOPO_FIN)
{
}

void RLoadTopoFin::Process(OpCode opCode, Stream * data, Client * caller)
{
	data->Clear();

	data->WriteInt16(0);
	data->WriteInt16(S_VISITED_SECTION_LIST);

	if (caller->_account->_selectedPlayer->_visitedSections.size() == 0)
		data->WriteInt32(0x00000800);
	else
	{
		data->WriteInt16(caller->_account->_selectedPlayer->_visitedSections.size());
		data->WriteInt16(8);
		data->WriteInt16(8);
		data->WriteInt16(24);

		for (size_t i = 0; i < caller->_account->_selectedPlayer->_visitedSections.size(); i++)
		{
			data->WriteInt32(caller->_account->_selectedPlayer->_visitedSections[i].continentId);
			data->WriteInt32(caller->_account->_selectedPlayer->_visitedSections[i].areaId);
			data->WriteInt32(caller->_account->_selectedPlayer->_visitedSections[i].sectionId);
		}
	}
	data->WritePos(0);
	caller->Send(data);
	data->Clear();

	data->WriteInt16(0);
	data->WriteInt16(S_REQUEST_INVITE_GUILD_TAG);
	data->WriteHexString("000000000000");
	data->WritePos(0);
	caller->Send(data);
	data->Clear();

	data->WriteInt16(0);
	data->WriteInt16(S_USER_BLOCK_LIST);
	data->WriteHexString("000000000000");
	data->WritePos(0);
	caller->Send(data);
	data->Clear();

	data->WriteInt16(0);
	data->WriteInt16(S_FRIEND_GROUP_LIST);
	data->WriteHexString("01000800080000001200020000004200650073007400200046007200690065006E006400730000000000");
	data->WritePos(0);
	caller->Send(data);
	data->Clear();

	data->WriteInt16(0);
	data->WriteInt16(S_FRIEND_LIST); 
	data->WriteInt32(0);
	short name_pos = data->_pos;
	data->WriteInt16(0);
	data->WriteInt16(0); //unk 5b00 91
	data->WritePos(name_pos);
	data->WriteString("Gazah Server");
	data->WriteInt16(0);
	data->WritePos(0);
	caller->Send(data);
	data->Clear();

	data->WriteInt16(0);
	data->WriteInt16(S_LOAD_CLIENT_USER_SETTING);

	if (caller->_account->_selectedPlayer->_userSettings)
		data->Write(caller->_account->_selectedPlayer->_userSettings, 
			caller->_account->_selectedPlayer->_settingSize);
	else
		data->WriteInt32(128);
	data->WritePos(0);
	caller->Send(data);
	data->Clear();


	data->WriteInt16(0);
	data->WriteInt16(S_TRADE_BROKER_CALC_NOTIFY);
	data->WriteHexString("00000000000000000000");
	data->WritePos(0);
	caller->Send(data);
	data->Clear();

	data->WriteInt16(0);
	data->WriteInt16(S_PARCEL_READ_RECV_STATUS);
	data->WriteHexString("00000000000000000000");
	data->WritePos(0);
	caller->Send(data);
	data->Clear();

	data->WriteInt16(0);
	data->WriteInt16(S_ONGOING_LEVEL_EVENT_LIST);
	data->WriteHexString("000000000000");
	data->WritePos(0);
	caller->Send(data);
	data->Clear();

	data->WriteInt16(0);
	data->WriteInt16(S_ONGOING_HUNTING_EVENT_LIST);
	data->WriteHexString("0100080008000000200000000000");
	data->WritePos(0);
	caller->Send(data);
	data->Clear();

	data->WriteInt16(0);
	data->WriteInt16(S_EP_SYSTEM_DAILY_EVENT_EXP_ON_OFF);
	data->WriteHexString("000000000000");
	data->WritePos(0);
	caller->Send(data);
	data->Clear();


	//------
#pragma region S_SKILL_LIST
	data->WriteInt16(0);
	data->WriteInt16(S_SKILL_LIST);

	data->WriteInt16(caller->_account->_selectedPlayer->_skillList.size());
	data->WriteInt16(8);
	data->WriteInt16(8);
	Skill* sk = nullptr;
	for (size_t i = 0; i < caller->_account->_selectedPlayer->_skillList.size(); i++)
	{
		sk = caller->_account->_selectedPlayer->_skillList[i];

		data->WriteInt16(sk->unk1);
		data->WriteInt32(sk->_skillId);
		data->WriteInt16(sk->unk2);
		data->WriteInt16(sk->unk3);
	}

	//data->WriteHexString("090008000800120074270000010012001C00844E000001001C002600A49C0000010026003000C4A28900010030003A00D4C9890001003A00440095A69903010044004E001127000000004E0058002C4C00000000580000002D4C00000000");

	data->WritePos(0);
	caller->Send(data);
	data->Clear();
#pragma endregion
	

	data->WriteInt16(0);
	data->WriteInt16(S_SPAWN_ME);

	data->WriteInt64(caller->_account->_selectedPlayer->_entityId);
	data->WriteFloat(caller->_account->_selectedPlayer->_position->_X);
	data->WriteFloat(caller->_account->_selectedPlayer->_position->_Y);
	data->WriteFloat(caller->_account->_selectedPlayer->_position->_Z);
	data->WriteInt16(caller->_account->_selectedPlayer->_position->_heading); //velika
	data->WriteInt16(caller->_account->_selectedPlayer->_stats->_currentHp > 0 ? 1 : 0); //alive
	data->WritePos(0);
	caller->Send(data);
	data->Clear();

#pragma region SEND_USER_STATS
	Player * p = caller->_account->_selectedPlayer;
	CreatureStats * stats = p->_stats;

	Stream  s = Stream();
	data->WriteInt16(0);
	data->WriteInt16(S_PLAYER_STAT_UPDATE); //S_PLAYER_STAT_UPDATE 

	data->WriteInt32(stats->_currentHp); //ACTUAL hp
	data->WriteInt32(stats->_currentMp); //ACTUAL mp
	data->WriteInt32(0);
	data->WriteInt32(stats->_maxHp);
	data->WriteInt32(stats->_maxMp);


	data->WriteInt32(stats->_power); //base power	[character power + items base power]
	data->WriteInt32(stats->_endurance); //base endurance [character endurance + items base endurance]				 
	data->WriteInt32(stats->_impactFactor);
	data->WriteInt32(stats->_balanceFactor);
	data->WriteInt16(stats->_movementSpeed);
	data->WriteInt16(50); //??							
	data->WriteInt16(stats->_attackSpeed);
	data->WriteFloat((float)stats->_critChance);

	data->WriteFloat((float)stats->_critResist);
	data->WriteFloat((float)stats->_critPower);

	data->WriteInt32(stats->_attack); 	//base attack range1				
	data->WriteInt32(stats->_attack); //base attack range2			

	data->WriteInt32(stats->_defense); //defense 1					
	data->WriteInt32(stats->_impact);	// impact		
	data->WriteInt32(stats->_balance);	//balance		

	data->WriteFloat(52.25); //RESISTANCES	weakening
	data->WriteFloat(52.25); //RESISTANCES	
	data->WriteFloat(52.25); //RESISTANCES	stun

	data->WriteInt32(11);  //aditional power [from bonuses and effects]			
	data->WriteInt32(8);   //aditional endurance [from bonuses and effects]
	data->WriteInt32(12);  //aditional impactFactor
	data->WriteInt32(13);  //aditional balanceFactor
	data->WriteInt32(14);  //aditional movementSpeed
	data->WriteInt32(15);  //aditional attackSpeed
	data->WriteInt32(5775); //aditional attack range1??
	data->WriteInt32(5776);	//aditional attack range2??
	data->WriteInt32(5777); //aditioanl impact ??
	data->WriteInt32(5777); //aditional defense range1
	data->WriteInt32(5777); //aditional defense range2

	data->WriteInt32(0);
	data->WriteInt32(0);
	data->WriteInt32(0);
	data->WriteInt32(0);
	data->WriteInt32(0);

	data->WriteInt16(0);	 //unk
	data->WriteInt32(p->_level);	 //unk
	data->WriteInt16(0);	 //unk

	data->WriteByte(1); //alive / combat ?

	data->WriteInt32(stats->_staminaHp);	 //aditioanl stamina hp
	data->WriteInt32(stats->_staminaMp);	 //aditioanl stamina mp
	data->WriteInt32(120);	 //stamina?
	data->WriteInt32(220);	 //max stamina?

	data->WriteInt32(1);	 //unk
	data->WriteInt32(2);	 //unk
	data->WriteInt32(3);	 //unk
	data->WriteInt32(0);	 //inafmy
	data->WriteInt32(177);	 //inventory item level
	data->WriteInt32(90);	 //warehouse item level
	data->WriteInt32(5);	 //unk
	data->WriteInt32(6);	 //unk
	data->WriteInt32(7);	 //unk
	data->WriteInt32(8);	 //unk
	data->WriteInt32(0);	 //unk

	data->WriteFloat(102.04);
	data->WriteFloat(0);	 //unk
	data->WriteFloat(1.0);	 //unk
	data->WritePos(0);

	caller->Send(data);
#pragma endregion


	WorldSystem::EnterWorld(caller);
}
