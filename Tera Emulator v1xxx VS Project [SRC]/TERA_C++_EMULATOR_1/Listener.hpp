#ifndef LISTENER_H
#define LISTENER_H

#include "socket.hpp"
#include "Client.hpp"

#include <thread>

class Server;
class Listener
{
public:
	Listener(const char* ip, unsigned int port, unsigned int maxClients);
	~Listener();

	const bool Initialize();

	const bool Start(Server* instance);
	const bool Stop();
	const bool IsValid();
private:
	static void Listen(SOCKET& socket, Server* instance);



private:

	const unsigned int _maxClients, _port;
	const char* _ip;
	static bool _listen;
	static bool _listening;
	static bool _initialized;
	std::thread _listenerThread;
	SOCKET _listeningSocket;


};

#endif