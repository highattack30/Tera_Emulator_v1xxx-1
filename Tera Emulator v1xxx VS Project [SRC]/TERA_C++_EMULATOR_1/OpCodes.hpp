#ifndef OPCODES_H
#define OPCODES_H

#include <vector>
#include <thread>
#include <mutex>

class SendPacket;
class OpCodes
{
public:
	OpCodes();
	~OpCodes();

	static void Clear();

	static SendPacket* Get(int);

	static bool Add(SendPacket* sendPacket);
	static unsigned int Count();
private:
	static std::vector<SendPacket*> _opCodesList;
	static std::mutex _opCodesMutex;

};

#endif