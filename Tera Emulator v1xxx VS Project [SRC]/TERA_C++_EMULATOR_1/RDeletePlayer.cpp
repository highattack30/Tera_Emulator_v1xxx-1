#include "RDeletePlayer.h"

RDeletePlayer::RDeletePlayer() : SendPacket(C_DELETE_USER)
{
}

void RDeletePlayer::Process(OpCode opCode, Stream * data, Client * caller)
{
	int id = data->ReadInt32();
	bool result = false;
	Player * p = caller->_account->GetPlayer(id);
	if (p)
	{
		if (caller->_account->GetPlayer(id)->_level > 40)
		{
			caller->_account->GetPlayer(id)->_banTimeUTC = ServerTimer::GetCurrentUTC() + 86400; //1 day
		}
		else
			caller->_account->GetPlayer(id)->_toDelete = true;

		result = true;
	}

	data->Clear();
	data->WriteInt16(5);
	data->WriteInt16(S_DELETE_USER);
	data->WriteByte(result == true ? 1 : 0);
	caller->Send(data);
	data->Clear();
}