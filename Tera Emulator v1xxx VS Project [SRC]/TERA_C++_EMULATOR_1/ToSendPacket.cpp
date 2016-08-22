#include "ToSendPacket.h"



ToSendPacket::ToSendPacket(Client * callerClient, Stream * data, BC_TYPE type,bool ordered, void ** args)
{
	_bcType = type;
	_args = args;
	_callerClient = callerClient;
	_data = data;
	_ordered = ordered;
}

ToSendPacket::~ToSendPacket()
{
	_args = 0;
	delete _data;
	_data = 0;
}
