#include "GuildSystem.h"
#include "Client.hpp"
#include "Player.hpp"
#include "Account.hpp"
#include "Stream.h"
#include "OpCodesEnum.h"

GuildSystem::GuildSystem()
{
}


GuildSystem::~GuildSystem()
{
}

void GuildSystem::BrodcastGuildFlag(Client * c)
{
	Stream s = Stream();

	s.WriteInt16(0);
	s.WriteInt16(S_BROCAST_GUILD_FLAG);

	s.WriteInt16(0);
	s.WriteInt16(8);
	s.WriteInt16(8);

	s.WritePos(0);

	c->Send(s._raw, s._size);
	s.Clear();
}
