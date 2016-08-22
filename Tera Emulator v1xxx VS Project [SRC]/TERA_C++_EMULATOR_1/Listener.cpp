#include "Listener.hpp"
#include "Server.hpp"


Listener::Listener(const char* ip, unsigned int port, unsigned int maxClients) :_port(port), _maxClients(maxClients)
{
	_ip = ip;
	_listen = false;
	_initialized = false;
}


Listener::~Listener()
{
	Stop();
}

const bool Listener::Initialize()
{

	_listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_listeningSocket == SOCKET_ERROR)
		return false;


	sockaddr_in sockData;
	sockData.sin_family = AF_INET;
	sockData.sin_addr.s_addr = inet_addr(_ip);
	sockData.sin_port = htons(_port);


	if (bind(_listeningSocket, (const sockaddr*)&sockData, sizeof sockaddr) != 0)
	{
		std::cout << "listener bind fail\n";
		closesocket(_listeningSocket);
		return false;
	}

	if (listen(_listeningSocket, SOMAXCONN) != 0)
	{
		std::cout << "listen  fail\n";
		closesocket(_listeningSocket);
		return false;
	}


	_initialized = true;
	return true;
}



const bool Listener::Start(Server* instance)
{
	if (_listen || _listening)
		return false;

	if (!IsValid())
	{
		if (!Initialize())
			return false;
	}

	_listen = true;
	_listenerThread = std::thread(Listen, std::ref(_listeningSocket), instance);
	_listenerThread.detach();

	return true;
}

const bool Listener::Stop()
{
	if (!_listening)
		return false;

	_listen = false;
	shutdown(_listeningSocket, SD_BOTH);
	closesocket(_listeningSocket);

	return true;
}

const bool Listener::IsValid()
{
	return _initialized;
}

void Listener::Listen(SOCKET& socket, Server* server)
{
	_listening = true;
	int length = sizeof(sockaddr);
	while (_listen)
	{
		SOCKET newConnection;
		sockaddr_in connectionData;

		newConnection = accept(socket, (sockaddr*)&connectionData, &length);
		if (newConnection != INVALID_SOCKET)
		{
			Client* con = new Client(newConnection, connectionData, server);
			server->NewConnection(con);
		}
	}

	_listening = false;
	_initialized = false;
	//std::cout << ">Listener closed!\n"; for DEBUGGING
}

bool Listener::_listen = false;
bool Listener::_listening = false;
bool Listener::_initialized = false;
