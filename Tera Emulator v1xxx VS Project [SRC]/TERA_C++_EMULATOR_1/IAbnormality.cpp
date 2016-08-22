#include "IAbnormality.h"
#include "Client.hpp"
#include "Effect.h"


IAbnormality::IAbnormality()
{
}


IAbnormality::~IAbnormality()
{
	for (size_t i = 0; i < _effects.size(); i++)
	{
		_effects[i] = 0;
	}
	_effects.clear();
}

void IAbnormality::ApplyAbnormality(Client * client)
{
	for (size_t i = 0; i < _effects.size(); i++)
	{
		if (_effects[i])
		{
			_effects[i]->ApplyEffect(client);
		}
	}
}

void IAbnormality::ApplyAbnormality(Creature * creature)
{
	for (size_t i = 0; i < _effects.size(); i++)
	{
		if (_effects[i])
		{
			_effects[i]->ApplyEffect(creature);
		}
	}
}

IAbnormality* IAbnormality::Clone()
{
	IAbnormality * nC = new IAbnormality();
	memcpy_s((void*)nC, sizeof IAbnormality, (const void*)this, sizeof IAbnormality);
	return nC;
}
