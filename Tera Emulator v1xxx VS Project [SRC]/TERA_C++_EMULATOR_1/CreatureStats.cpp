#include "CreatureStats.h"



CreatureStats::CreatureStats()
{
	memset((void*)this, 0, sizeof CreatureStats);
}


CreatureStats::~CreatureStats()
{
	for (size_t i = 0; i < _bonusList.size(); i++)
	{

	}
}

CreatureStats * CreatureStats::Clone()
{
	CreatureStats * out = new CreatureStats();
	memset((void*)out, 0, sizeof CreatureStats);
	memcpy_s((void*)out, sizeof CreatureStats, (const void*)this, sizeof CreatureStats);
	out->_isClone = true;
	return out;
}
