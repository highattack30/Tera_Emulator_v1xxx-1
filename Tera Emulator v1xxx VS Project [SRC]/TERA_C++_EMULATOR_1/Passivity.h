#pragma once

#include <vector>
#include <string>

enum PassivityType : int
{
	STATS_CHANGE = 0
};

class Effect; class Client; class Creature; class IAbnormality;
class Passivity
{
public:
	Passivity(PassivityType type);
	virtual ~Passivity();

	virtual void ApplyPassivity(Client* client) = 0;
	virtual void ApplyPassivity(Creature* creature) = 0;

	std::vector<Effect*> _effects;
	std::vector<IAbnormality*> _abnormalities;
	PassivityType _type;
};

