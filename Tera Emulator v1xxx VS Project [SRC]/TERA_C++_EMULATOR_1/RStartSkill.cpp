#include "RStartSkill.h"
#include "Client.hpp"
#include "Account.hpp"
#include "Player.hpp"
#include "WorldPosition.h"
#include "Skill.h"

RStartSkill::RStartSkill() : SendPacket(C_START_SKILL)
{
}

void RStartSkill::Process(OpCode opCode, Stream * stream, Client * caller)
{
	Player * player = caller->_account->_selectedPlayer;
	int skillId = stream->ReadInt32();

	short unk1 = stream->ReadInt16();

	float x = stream->ReadFloat();
	float y = stream->ReadFloat();
	float z = stream->ReadFloat();
	short heading = stream->ReadInt16();

	int unk2 = stream->ReadInt32();
	int unk3 = stream->ReadInt32();
	int unk4 = stream->ReadInt32();
	int unk5 = stream->ReadInt32();
	int unk6 = stream->ReadInt32();
	int unk7 = stream->ReadInt32();

	
	stream->Clear();
	stream->WriteInt16(0);
	stream->WriteInt16(S_ACTION_STAGE);

	stream->WriteInt64(player->_entityId);

	stream->WriteFloat(x);
	stream->WriteFloat(y);
	stream->WriteFloat(z);
	stream->WriteInt16(player->_position->_heading);

	stream->WriteInt32(player->_model);

	stream->WriteInt32(skillId);
	stream->WriteInt32(0);

	stream->WriteFloat(1.8f); //the speed multiplyer
	stream->WriteInt32(0);
	stream->WriteFloat(1.2f);

	stream->WriteInt32(0);
	stream->WriteInt32(0);
	stream->WriteInt32(0);
	stream->WriteInt32(0);
	stream->WriteInt32(0);

	stream->WriteByte(0);
	stream->WritePos(0);
	
	//todo send skill_use to visible clients

	caller->Send(stream);
}


