#include "RMovePlayer.h"


RMovePlayer::RMovePlayer() : SendPacket(C_PLAYER_LOCATION)
{
}

void RMovePlayer::Process(OpCode opCode, Stream * stream, Client * caller)
{
	float x = stream->ReadFloat();
	float y = stream->ReadFloat();
	float z = stream->ReadFloat();
	int w = stream->ReadInt32();
	float x2 = stream->ReadFloat();
	float y2 = stream->ReadFloat();
	float z2 = stream->ReadFloat();
	int type = stream->ReadInt32();
	short speed = stream->ReadInt16();
	byte unk = stream->ReadByte();
	int time = stream->ReadInt32();


	caller->_account->_selectedPlayer->_position->_X = x2;
	caller->_account->_selectedPlayer->_position->_Y = y2;
	caller->_account->_selectedPlayer->_position->_Z = z2;
	caller->_account->_selectedPlayer->_position->_heading = w;

	stream->Clear();
	stream->WriteInt16(0);
	stream->WriteInt16(S_USER_LOCATION);
	stream->WriteInt64(caller->_account->_selectedPlayer->_entityId);
	stream->WriteFloat(x);
	stream->WriteFloat(y);
	stream->WriteFloat(z);
	stream->WriteInt16(w);
	stream->WriteInt16(speed);
	stream->WriteFloat(x2);
	stream->WriteFloat(y2);
	stream->WriteFloat(z2);
	stream->WriteInt32(type);
	stream->WriteByte(0); //unk
	stream->WritePos(0);

	switch (type)
	{
	case P_MOVE_NORMAL:
	{
		BroadcastSystem::Broadcast(caller, stream, VISIBLE_CLIENTS, 0);
	}break;

	case P_JUMP_START:
	{
		//todo
	}break;

	case P_JUMP_END:
	{
		//todo
	}break;

	}


}

