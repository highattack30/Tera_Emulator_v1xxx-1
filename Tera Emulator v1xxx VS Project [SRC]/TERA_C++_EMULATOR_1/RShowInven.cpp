#include "RShowInven.h"

RShowInven::RShowInven() : SendPacket(C_SHOW_INVEN)
{
}

void RShowInven::Process(OpCode opCode, Stream * data, Client * caller)
{
	data->WriteInt16(0);
	data->WriteInt16(S_INVEN);
	data->WriteInt32(0);
	data->WriteInt64(caller->_account->_selectedPlayer->_entityId);

	data->WriteInt64(10);
	data->WriteByte(0);
	data->WriteByte(0);
	data->WriteByte(0);

	data->WriteInt32(8);
	data->WriteInt32(145);
	data->WriteInt32(551);
	data->WriteInt32(0);
	data->WriteInt32(0);
	data->WriteInt32(0);
	data->WriteInt32(0);
	data->WriteInt32(0);

	data->WritePos(0);
	BroadcastSystem::Broadcast(caller, data, ME, 0);
	data->Clear();
}
