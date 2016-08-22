#include "AbnormalitiesService.h"
#include "EffectService.h"
#include "Effect.h"
#include "IAbnormality.h"
#include "Stream.h"


const bool AbnormalitiesService::LoadAbnormalities(const char * file)
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
		IAbnormality * newAb = new IAbnormality;
		memset((void*)newAb, 0, sizeof IAbnormality);

		newAb->_id = s->ReadInt32();
		newAb->_name = s->ReadASCIIString().c_str();
		newAb->_lifeTime = s->ReadFloat();
		int efCount = s->ReadInt32();
		for (size_t j = 0; j < efCount; j++)
		{
			newAb->_effects.push_back(EffectService::ResolveEffectId(s->ReadInt32()));
		}
		_abnormalities.push_back(newAb);
	}
	//then abnormalities that apllyes effects on target ...like buffs and debufss, stances..etc
	s->Clear();

	return true;
}

void AbnormalitiesService::Release()
{
	for (size_t i = 0; i < _abnormalities.size(); i++)

	{
		if (_abnormalities[i])
		{
			delete _abnormalities[i];
			_abnormalities[i] = 0;
		}
	}
}

unsigned int AbnormalitiesService::GetCount()
{
	return _abnormalities.size();
}

std::vector <IAbnormality *> AbnormalitiesService::_abnormalities;