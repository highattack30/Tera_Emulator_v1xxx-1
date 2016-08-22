#ifndef SKILL_H
#define SKILL_H

#include <vector>
#include <string>

enum SkillType : int
{
	EMOTE,
	PROJECTILE,
	HEAL,
	DEBUFF,
	BUFF,
	DAMAGE,
	BLOCK,
	DAMAGE_ABSORPTION,
	EVADE,
	EFFECT,
	SUMMON,
	STANCE,

};

enum Affecting :short
{
	A_ME,
	A_ENEMY,
	A_PARTY,
	A_GUILD,
	A_RAID,
	A_ALL
};
class Effect;
class Skill
{
public:
	Skill();
	int _id;
	int _skillId;
	short unk1;
	short unk2;
	short unk3;


	float _castTime;
	float _cooldown;
	int		_mpCost;
	SkillType _type;
	Affecting _affecting;
	std::string _name;
	Skill* Clone();
};

#endif