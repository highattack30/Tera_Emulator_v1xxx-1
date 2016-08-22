#ifndef EFFECT_SERVICE_H
#define EFFECT_SERVICE_H

#include <vector>
#include <mutex>
#include <fstream>

class Effect;
class EffectService
{
public:
	static const bool LoadEffects(const char * file = "data/effects.bin");

	static Effect * ResolveEffectId(int id);

	static void Release();
	static unsigned int GetCount();
private:
	static std::vector<Effect *> _effects;
};

#endif