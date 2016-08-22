#include "RCancelSkill.h"

RCancelSkill::RCancelSkill() : SendPacket(C_CANCEL_SKILL)
{
}

void RCancelSkill::Process(OpCode opCode, Stream * stream, Client * caller)
{
	int skill = stream->ReadInt32();
	int type = stream->ReadInt32();

	Player * player = caller->_account->_selectedPlayer;

	stream->Clear();
	stream->WriteInt16(42);
	stream->WriteInt16(S_ACTION_END);
	stream->WriteInt64(player->_entityId);

	stream->WriteFloat(player->_position->_X);
	stream->WriteFloat(player->_position->_Y);
	stream->WriteFloat(player->_position->_Z);
	stream->WriteInt16(player->_position->_heading);

	stream->WriteInt32(player->_model);
	stream->WriteInt32(skill);
	stream->WriteInt32(0);
	stream->WriteInt32(0);

	caller->Send(stream);
}