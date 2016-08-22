#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <mutex>

#include "socket.hpp"

#include "Listener.hpp"
#include "Client.hpp"

#include "mysql\MySqlDriver.h"

#define SERVER_ID  1

class Server
{
private:
	struct Config
	{
		std::string ip;
		unsigned int port;
		unsigned int maxClients;
		bool isValid;

		std::string mysqlHostAddress;
		std::string mysqlUsername;
		std::string mysqlPassword;
		std::string database;
	};

	friend class Listener;
	friend class Client;
public:
	Server();
	~Server();

public:
	const bool Initialize();

	const bool Run();
	const bool Shutdown();

private:
	void NewConnection(Client* connection);
	void EndConnection(Client* connection);
	unsigned int _connectedClients;
private:
	Config LoadConfigFromFile(const char* file);
private:
	bool _started;
	WSADATA _wsaData;
	MySqlDriver * _mysqlDriver;
	Listener * _listener;
	Config _serverConfig;
	std::vector<Client*> _clients;
	std::mutex _serverMutex;
};

#endif
