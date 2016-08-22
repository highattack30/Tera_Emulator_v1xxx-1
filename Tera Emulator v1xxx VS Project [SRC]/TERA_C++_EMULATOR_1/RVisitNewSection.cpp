#include "RVisitNewSection.h"


RVisitNewSection::RVisitNewSection() : SendPacket(C_VISIT_NEW_SECTION)
{
}

void RVisitNewSection::Process(OpCode opCode, Stream * data, Client * caller)
{
	int unk1 = data->ReadInt32();
	int unk2 = data->ReadInt32();
	int areaId = data->ReadInt32();


	Player::AreaSection temp = Player::AreaSection(unk1, unk2, areaId);
	Player * p = 0;
	while (!(p = caller->LockPlayer()))
		continue;

	bool b = false;
	for (size_t i = 0; i < p->_visitedSections.size(); i++)
	{
		if (p->_visitedSections[i] == temp)
		{
			b = true;
			break;
		}
	}

	if (b)
	{
		p->_visitedSections.push_back(temp);
	}

	p->_currentVisitedSection = temp;

	caller->UnlockPlayer();

	data->Clear();
	data->WriteInt16(19);
	data->WriteInt16(S_VISIT_NEW_SECTION);
	data->WriteByte(0);
	data->WriteInt32(unk1);
	data->WriteInt32(unk2);
	data->WriteInt32(areaId);
	data->WriteInt16(0);
	BroadcastSystem::Broadcast(caller, data, ME, 0);
}

