#include "Skill.h"



Skill::Skill()
{
}

Skill * Skill::Clone()
{
	Skill * out = new Skill();

	memcpy_s((void*)out, sizeof Skill, (const void*)this, sizeof Skill);
	out->_name = _name;

	return out;
}
