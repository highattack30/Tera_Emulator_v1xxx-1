#include "RHardwareInfo.h"
#include "Client.hpp"
#include "Account.hpp"


RHardwareInfo::RHardwareInfo() : SendPacket(C_HARDWARE_INFO)
{

}

void RHardwareInfo::Process(OpCode opCode, Stream * data, Client * caller)
{
	data->_pos = 55;
	caller->_account->_hardwareInfo = data->ReadString();
}
