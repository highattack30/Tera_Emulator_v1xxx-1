#include "RSkillLearnRequest.h"

RSkillLearnRequest::RSkillLearnRequest() : SendPacket(C_SKILL_LEARN_REQUEST)
{
}

void RSkillLearnRequest::Process(OpCode opCode, Stream * stream, Client * caller)
{
	int unk1 = stream->ReadInt32();
	short skillId = stream->ReadInt16();
	short unk2 = stream->ReadInt16();
	byte unk3 = stream->ReadByte();

	stream->Clear();
	stream->WriteInt16(14);
	stream->WriteInt16(S_SKILL_LEARN_RESULT);
	stream->WriteInt32((int)unk3);
	stream->WriteInt16(skillId);
	stream->WriteInt16(unk2);
	stream->WriteInt16(0);
	BroadcastSystem::Broadcast(caller, stream, ME, 0);
}
