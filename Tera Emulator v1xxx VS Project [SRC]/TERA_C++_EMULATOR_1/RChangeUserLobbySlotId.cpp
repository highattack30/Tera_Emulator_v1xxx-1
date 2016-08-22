#include "RChangeUserLobbySlotId.h"
#include "Client.hpp"
#include "Account.hpp"
#include "Player.hpp"


RChangeUserLobbySlotId::RChangeUserLobbySlotId() : SendPacket(C_CHANGE_USER_LOBBY_SLOT_ID)
{
}

void RChangeUserLobbySlotId::Process(OpCode opCode, Stream * stream, Client * caller)
{
	short id1 = stream->ReadInt16();
	stream->_pos = 22;
	short id2 = stream->ReadInt16();

	int i1 = -1, i2 = -1;
	for (size_t i = 0; i < caller->_account->_playerList.size(); i++)
	{
		if (caller->_account->_playerList[i]->_lobbyId == (byte)id1)
			i1 = i;
		else if (caller->_account->_playerList[i]->_lobbyId == (byte)id2)
			i2 = i;

	}


	byte temp = caller->_account->_playerList[i1]->_lobbyId;
	caller->_account->_playerList[i1]->_lobbyId = caller->_account->_playerList[i2]->_lobbyId;
	caller->_account->_playerList[i2]->_lobbyId = temp;
}