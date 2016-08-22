#include "RCanCreatePlayer.h"
#include "PlayerService.h"
#include "Client.hpp"
#include "Account.hpp"

RCanCreatePlayer::RCanCreatePlayer() : SendPacket(C_CAN_CREATE_USER)
{
}

void RCanCreatePlayer::Process(OpCode opCode, Stream * data, Client * caller)
{
	data->Clear();
	data->WriteInt16(5);
	data->WriteInt16(S_CAN_CREATE_USER);
	data->WriteByte((unsigned char)PlayerService::CanCreatePlayer(caller->_account));
	caller->Send(data);
	data->Clear();
}