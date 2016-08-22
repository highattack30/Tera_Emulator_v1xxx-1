#ifndef CLIENT_H
#define CLIENT_H

#include <thread>
#include <fstream>
#include <random>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <mutex>
#include <concurrent_queue.h>

#include "socket.hpp"
#include "TeraPacket.hpp"
#include "AbnormalityManager.h"
#include "Entity.h"

class Account;
class Player;
class Server;
class Stream;
class Area;

class Client : public Entity
{
	friend class WorldObject;
	friend class Creature;
	friend class PlayerService;
	friend class WorldSystem;
	friend class BroadcastSystem;
	friend class SendPacket;
public:
	Client(SOCKET socket, sockaddr_in sockData, Server* server);
	~Client();

	void Close();
	void SetId(int id);
	Player* LockPlayer();
	const bool IsLocked();
	void UnlockPlayer();
	const bool Send(byte* data, unsigned int length);
	const bool Send(Stream * s);
	const bool IsVisibleClient(Client * c);
	void RemoveVisibleClient(Client * c);
	void AddVisibleClient(Client* c);
	void Dump(byte* data, unsigned int length, bool recv);
private:
	static void Run(Client * instance, Server* server);
	static void Recevie(Client * instance);
	static const bool ProcessData(Client * instance, TeraPacket* packet, Stream * processStream);


public:
	Account* _account;

private:
	bool _playerLocked;
	std::thread _clientThread;
	std::thread _sendThread;
	bool _run;
	bool _opened;
	std::mutex _sendMutex;
	std::mutex _recvMutex;
	std::condition_variable _canRecv;
	bool _canRecvVariable;
	Crypt::Session *_session;
	std::fstream _file;
	SOCKET _socket;
	sockaddr_in _sockData;
	int _connectionId;
	std::vector<Client*> _visibleClients;
};





#endif