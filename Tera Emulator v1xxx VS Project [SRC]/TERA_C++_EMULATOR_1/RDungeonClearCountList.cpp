#include "RDungeonClearCountList.h"



RDungeonClearCountList::RDungeonClearCountList() :SendPacket(C_DUNGEON_CLEAR_COUNT_LIST)
{
}

void RDungeonClearCountList::Process(OpCode opCode, Stream * data, Client * caller)
{
	data->ReadInt16();
	std::string _playerName = data->ReadReceivedString();

	data->Clear();
	data->WriteInt16(8);
	data->WriteInt16(S_DUNGEON_CLEAR_COUNT_LIST);
	data->WriteInt32(0); //todo

	BroadcastSystem::Broadcast(caller, data, ME, 0);
}