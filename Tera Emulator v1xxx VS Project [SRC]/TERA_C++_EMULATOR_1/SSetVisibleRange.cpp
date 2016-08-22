#include "SSetVisibleRange.h"
#include "Client.hpp"
#include "Account.hpp"

SSetVisibleRange::SSetVisibleRange() : SendPacket(C_SET_VISIBLE_RANGE)
{
}

void SSetVisibleRange::Process(OpCode opCode, Stream * data, Client * caller)
{
	caller->_account->_visibleRange = data->ReadInt32();
}