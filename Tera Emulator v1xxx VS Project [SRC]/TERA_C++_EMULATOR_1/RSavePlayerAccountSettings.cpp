#include "RSavePlayerAccountSettings.h"

RSavePlayerAccountSettings::RSavePlayerAccountSettings() : SendPacket(C_SAVE_CLIENT_ACCOUNT_SETTING)
{
}

void RSavePlayerAccountSettings::Process(OpCode opCode, Stream * stream, Client * caller)
{
	if (caller->_account->_accountSettings)
	{
		delete caller->_account->_accountSettings;
		caller->_account->_accountSettings = 0;
	}
	caller->_account->_accountSettingsSize = (unsigned short)stream->_size;
	caller->_account->_accountSettings = new byte[stream->_size];
	stream->Read(caller->_account->_accountSettings, stream->_size);

	//after this packet it crushes....the crypt system [was]
}