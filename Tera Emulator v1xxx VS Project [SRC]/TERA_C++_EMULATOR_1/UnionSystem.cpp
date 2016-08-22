#include "UnionSystem.h"

void UnionSystem::SendElectionState(Client * client)
{
	Stream s = Stream();

	s.WriteInt16(0);
	s.WriteInt16((int)S_CURRENT_ELECTION_STATE);

	s.WriteHexString("000000000B000000000000000000FF3CE451000000000000");//todo
	s.WritePos(0);

	client->Send(s._raw, s._size);
}
