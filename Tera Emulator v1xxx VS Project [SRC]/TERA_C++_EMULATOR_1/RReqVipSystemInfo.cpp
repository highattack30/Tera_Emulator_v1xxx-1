#include "RReqVipSystemInfo.h"
#include "Client.hpp"
#include "Account.hpp"


RReqVipSystemInfo::RReqVipSystemInfo() : SendPacket(C_REQUEST_VIP_SYSTEM_INFO)
{
}

void RReqVipSystemInfo::Process(OpCode opCode, Stream * data, Client * caller)
{
	data->Clear();

	data->WriteInt16(0);
	data->WriteInt16(S_SEND_VIP_SYSTEM_INFO);

	bool a = caller->_account->_selectedPlayer == 0 ? false : true;

	if (a)
		data->WriteHexString("2D0001000000000000000000000000000000000000000001A60100000000000000000000000000000100000000");
	else
		data->WriteHexString("2D0001000000000000000000000000000000000000000001B60100000000000000000000000000000100000000");

	data->WritePos(0);
	caller->Send(data);
}


