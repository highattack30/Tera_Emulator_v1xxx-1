#include "RRequestUserItemLevelInfo.h"



RRequestUserItemLevelInfo::RRequestUserItemLevelInfo() :SendPacket(C_REQUEST_USER_ITEMLEVEL_INFO)
{
}

void RRequestUserItemLevelInfo::Process(OpCode opCode, Stream * stream, Client * caller)
{
	stream->Clear();
	stream->WriteInt16(16);
	stream->WriteInt16(S_USER_ITEMLEVEL_INFO);
	stream->WriteInt32(12); //warehouse itemlevel
	stream->WriteInt32(15); //inventory and warehouse itemlevel
	stream->WriteInt16(0);

	BroadcastSystem::Broadcast(caller, stream, ME, 0);
}