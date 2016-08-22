#include "Effect.h"
#include "Client.hpp"
#include "Player.hpp"

const bool Effect::ApplyEffect(Client * client)
{
	int times = 0;
	Player *p = nullptr;
	while (!(p = client->LockPlayer()))
	{
		times++;
		if (times >= EFFECT_TIME_OUT)
			return false;
	}

	InternalApplyEffect(client, p);

	client->UnlockPlayer();
	return true;
}

const bool Effect::ApplyEffect(Creature * c)
{
	return true;
}

const bool Effect::InternalApplyEffect(Client * client, Player * p)
{
	return false;
}

const bool Effect::InternalApplyEffect(Creature * c)
{
	return false;
}
