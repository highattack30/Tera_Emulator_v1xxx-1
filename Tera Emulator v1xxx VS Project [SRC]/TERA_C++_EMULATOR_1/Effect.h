#ifndef EFFECT_H
#define EFFECT_H

#define EFFECT_TIME_OUT 5000
enum EffectType : int
{
	STUN = 0,
	DAMAGE = 1,
	DROPOUT = 2,
};

class Client; class Player; class Account; class IAbnormality; class Creature;
class Effect
{
public:
	const bool ApplyEffect(Client *client);
	const bool ApplyEffect(Creature * c);

protected:
	virtual const bool InternalApplyEffect(Client *client, Player* p);
	virtual const bool InternalApplyEffect(Creature * c);

public:
	int _id;
	float _value;
	EffectType _type;
};

#endif