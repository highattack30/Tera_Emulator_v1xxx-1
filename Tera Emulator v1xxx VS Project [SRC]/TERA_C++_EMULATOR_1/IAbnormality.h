#ifndef IABNORMALITY_H
#define IABNORMALITY_H

#include <vector>

class Client; class Player; class Account; class AbnormalityManager; class Effect; class Creature;
class IAbnormality
{
public:
	IAbnormality();
	virtual  ~IAbnormality();

	virtual void ApplyAbnormality(Client * client);
	virtual void ApplyAbnormality(Creature * creature);

	virtual IAbnormality* Clone();

	int  _id;
	const char * _name;
	float _lifeTime;
	float _startTime;
	float _currentTime;
	bool _alive;
	std::vector<Effect * >_effects;
};

#endif