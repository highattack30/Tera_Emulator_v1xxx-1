#ifndef PACKET_H
#define PACKET_H

#include <mutex>
#include "Crypt\data_type.hpp"
#include "Crypt\TeraCrypt.hpp"
#include "ServerUtils.hpp"

#include "socket.hpp"

class TeraPacket
{
public:
	TeraPacket();
	~TeraPacket();

	const bool GetPacket(SOCKET sock, Crypt::Session * session);

	void Resize(unsigned short size);
	void Reset();

	byte _opCode[2];
	byte* _raw;
	unsigned short _size;
	unsigned short _pos;

	const bool IsValidPacket();
	std::mutex _recvMutex;

};

#endif