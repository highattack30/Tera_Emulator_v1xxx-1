#include "RSaveUserSettings.h"

RSaveUserSettings::RSaveUserSettings() : SendPacket(C_SAVE_CLIENT_USER_SETTING)
{
}

void RSaveUserSettings::Process(OpCode opCode, Stream * stream, Client * owner)
{
	owner->_account->_selectedPlayer->_settingSize = stream->_size;
	if (owner->_account->_selectedPlayer->_userSettings)
	{
		delete owner->_account->_selectedPlayer->_userSettings;
		owner->_account->_selectedPlayer->_userSettings = 0;
	}

	owner->_account->_selectedPlayer->_userSettings = new byte[stream->_size];
	stream->Read(owner->_account->_selectedPlayer->_userSettings, stream->_size);
}