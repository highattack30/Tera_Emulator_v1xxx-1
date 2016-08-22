#include "RDungeonCoolTimeList.h"

RDungeonCoolTimeList::RDungeonCoolTimeList() : SendPacket(C_DUNGEON_COOL_TIME_LIST )
{
}

void RDungeonCoolTimeList::Process(OpCode opCode, Stream * data, Client * caller)
{
	data->Clear();
	data->WriteInt16(8);
	data->WriteInt16(S_DUNGEON_COOL_TIME_LIST);
	data->WriteInt32(0);
	BroadcastSystem::Broadcast(caller, data, ME, 0);
}