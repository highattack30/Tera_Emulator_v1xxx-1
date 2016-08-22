#include "RNpcGuildList.h"


RNpcGuildList::RNpcGuildList() : SendPacket(C_NPCGUILD_LIST)
{
}

void RNpcGuildList::Process(OpCode opCode, Stream * data, Client * caller)
{
	data->_pos += 2; //base int16 offset value
	std:: string _playerName = data->ReadReceivedString();

	data->Clear();
	data->WriteInt16(8);
	data->WriteInt16(S_NPCGUILD_LIST);
	data->WriteInt32(0); //todo
	BroadcastSystem::Broadcast(caller, data, ME, 0);
}