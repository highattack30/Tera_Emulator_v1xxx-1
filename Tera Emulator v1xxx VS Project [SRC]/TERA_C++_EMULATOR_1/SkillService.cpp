#include "SkillService.h"
#include "Skill.h"
#include "Stream.h"
#include "Client.hpp"
#include "Account.hpp"
#include "Player.hpp"

void SkillService::Release()
{
	for (size_t i = 0; i < _skills.size(); i++)
	{
		if (_skills[i])
		{
			delete _skills[i];
			_skills[i] = 0;
		}
	}
	_skills.clear();
}

const bool SkillService::LoadSkills(const char * file)
{
	std::ifstream fs;
	fs.open(file, std::ifstream::ate | std::ifstream::binary);
	if (!fs.is_open())
		return false;

	int size = fs.tellg();
	fs.seekg(0, std::ios::beg);

	Stream * s = new Stream(size);
	fs.read((char*)s->_raw, size);

	fs.close();

	int count = s->ReadInt32();

	for (size_t i = 0; i < count; i++)
	{
		Skill * newS = new Skill();
		newS->_id = s->ReadInt32();
		newS->_skillId = s->ReadInt32();
		newS->unk1 = s->ReadInt16();
		newS->unk2 = s->ReadInt16();
		newS->unk3 = s->ReadInt16();
		newS->_castTime = s->ReadFloat();
		newS->_type = (SkillType)s->ReadInt32();
		newS->_cooldown = s->ReadFloat();
		newS->_mpCost = s->ReadInt32();
		newS->_name = s->ReadASCIIString();

		_skills.push_back(newS);
	}

	s->Clear();
	//we read all skils from file when server started
	return true;
}


Skill * SkillService::ResolveSkill(int id)
{
	Skill * out = 0;

	for (size_t i = 0; i < _skills.size(); i++)
	{
		if (_skills[i] && _skills[i]->_id == id)
		{
			out = _skills[i];
			break;
		}
	}

	return out;
}

void SkillService::ResolveSkillSet(std::vector<int>& ids, std::vector<Skill*>& skillSet)
{
	for (size_t i = 0; i < ids.size(); i++)
	{
		Skill* k = ResolveSkill(ids[i]);
		if (k)
		{
			skillSet.push_back(k->Clone());
		}
	}
}

std::vector<Skill*> SkillService::_skills;