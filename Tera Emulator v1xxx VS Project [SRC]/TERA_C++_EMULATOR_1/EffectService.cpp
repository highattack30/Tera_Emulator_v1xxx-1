#include "EffectService.h"
#include "Stream.h"
#include "Effect.h"

const bool EffectService::LoadEffects(const char * file)
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
		Effect * newEf = new Effect;
		memset((void*)newEf, 0, sizeof Effect);

		newEf->_id = s->ReadInt32();
		newEf->_value = s->ReadFloat();
		newEf->_type = (EffectType)s->ReadInt32();

		_effects.push_back(newEf);
	}

	s->Clear();
	 //same with effects, a big file with all kid of effects [stagger effect....um...all kindof  effects]
	return true;
}

Effect * EffectService::ResolveEffectId(int id)
{
	for (size_t i = 0; i < _effects.size(); i++)
	{
		if (_effects[i] && _effects[i]->_id == id)
			return _effects[i];
	}
}

void EffectService::Release()
{
	for (size_t i = 0; i < _effects.size(); i++)

	{
		if (_effects[i])
		{
			delete _effects[i];
			_effects[i] = 0;
		}
	}
}

unsigned int EffectService::GetCount()
{
	return _effects.size();
}

std::vector<Effect *> EffectService::_effects;