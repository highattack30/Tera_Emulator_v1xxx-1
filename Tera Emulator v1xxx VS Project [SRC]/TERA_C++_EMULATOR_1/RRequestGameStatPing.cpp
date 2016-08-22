#include "RRequestGameStatPing.h"



RRequestGameStatPing::RRequestGameStatPing() : SendPacket(C_REQUEST_GAMESTAT_PING)
{
}

void RRequestGameStatPing::Process(OpCode opCode, Stream * data, Client * caller)
{
	data->Clear();
	data->WriteInt16(4);
	data->WriteInt16(S_RESPONSE_GAMESTAT_PONG);
	BroadcastSystem::Broadcast(caller, data, ME, 0);
}
