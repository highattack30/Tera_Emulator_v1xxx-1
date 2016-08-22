
#include "OpCodesEnum.h"
#include "SendPacket.h"

#include "OpCodes.hpp"

OpCodes::OpCodes()
{
}


OpCodes::~OpCodes()
{

}

void OpCodes::Clear()
{
	_opCodesMutex.lock();

	for (size_t i = 0; i < _opCodesList.size(); i++)
	{
		if (_opCodesList[i])
		{
			delete _opCodesList[i];
			_opCodesList[i] = 0;
		}
	}
	_opCodesList.clear();

	_opCodesMutex.unlock();
}


SendPacket * OpCodes::Get(int opCode)
{
	std::lock_guard<std::mutex> _lock(_opCodesMutex);
	SendPacket* out_packet = 0;

	for (size_t i = 0; i < _opCodesList.size(); i++)
	{
		if (_opCodesList[i])
		{
			if (opCode == (int)_opCodesList[i]->_toRecv)
			{
				out_packet = _opCodesList[i];
				break;
			}
		}
	}

	
	return out_packet;
}

bool OpCodes::Add(SendPacket * sendPacket)
{
	if (!sendPacket)
		return false;
	_opCodesList.push_back(sendPacket);
	return true;
}

unsigned int OpCodes::Count()
{
	return _opCodesList.size();
}

std::vector<SendPacket*> OpCodes::_opCodesList;
std::mutex OpCodes::_opCodesMutex;