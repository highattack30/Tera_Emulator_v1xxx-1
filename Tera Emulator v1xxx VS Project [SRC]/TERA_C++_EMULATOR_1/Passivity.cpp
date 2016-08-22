#include "Passivity.h"



Passivity::Passivity(PassivityType type)
{
	_type = type;
}


Passivity::~Passivity()
{
	for (size_t i = 0; i < _effects.size(); i++)
	{
		delete _effects[i];
		_effects[i] = 0;
	}
	_effects.clear();
}
