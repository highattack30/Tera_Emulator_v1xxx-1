#ifndef SKILLSERVICE_H
#define SKILLSERVICE_H

#include <vector>
#include <mutex>

class Client; class Account; class Player; class Skill;
class SkillService
{
public:
	
	static void Release();

	static const bool LoadSkills(const char* dir = ".//data//skills.bin");

	static void SendSkillList(Client* client);

	static Skill* ResolveSkill(int id);
	static void ResolveSkillSet(std::vector<int>& ids, std::vector<Skill*> & skillSet);


	static std::vector<Skill*> _skills;
};

#endif
