#include "Server.hpp"
#include "OpCodes.hpp"
#include "BroadcastService.h"

#include "PlayerService.h"
#include "InventoryService.h"
#include "SkillService.h"
#include "ServerTimer.h"
#include "AbnormalitiesService.h"
#include "EffectService.h"
#include "WorldSystem.h"

#include "SLoginArbiter.h"
#include "SCheckVersion.h"
#include "SSetVisibleRange.h"
#include "RGetPlayerList.h"
#include "RCanCreatePlayer.h"
#include "RStrEvaluateList.h"
#include "RCheckUsername.h"
#include "RCreatePlayer.h"
#include "RHardwareInfo.h"
#include "RReqVipSystemInfo.h"
#include "RSelectPlayer.h"
#include "RMovePlayer.h"
#include "RSavePlayerAccountSettings.h"
#include "RGetPlayerGuildLogo.h"
#include "RDeletePlayer.h"
#include "RChangeUserLobbySlotId.h"
#include "RCancelDeletePlayer.h"
#include "RLoadTopoFin.h"
#include "RTradeBrokerHighestItemLevel.h"
#include "RUnionSummary.h"
#include "RServerTime.h"
#include "RUpdateContentsPlayTime.h"
#include "RSimpleTipRepeatCheck.h"
#include "RSaveUserSettings.h"
#include "RRequestInstoreProductList.h"
#include "RGuardPkPolicy.h"
#include "RDungeonCoolTImeList.h"
#include "RRequestUserItemLevelInfo.h"
#include "RDungeonClearCountList.h"
#include "RNpcGuildList.h"
#include "RRaignInfo.h"
#include "RVisitNewSection.h"
#include "REventGuide.h"
#include "RShowInven.h"
#include "RSkillLearnRequest.h"
#include "RSkillLearnList.h"
#include "RStartSkill.h"
#include "RCancelSkill.h"
#include "RNotifyLocationInAction.h"
#include "RRequestGameStatPing.h"
#include "RExit.h"


Server::Server()
{
	_listener = 0;
	_connectedClients = 0;
	_mysqlDriver = 0;
}

Server::~Server()
{

	if (_listener)
	{
		_listener->Stop();
		delete _listener;
		_listener = 0;
	}
	SkillService::Release();
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i])
		{
			_clients[i]->Close();
			delete _clients[i];
			_clients[i] = 0;
		}
	}
	_clients.clear();



	WorldSystem::ReleaseData();

	PlayerService::ReleaseData();
	InventoryService::Release();
	AbnormalitiesService::Release();
	EffectService::Release();



	OpCodes::Clear();
	ServerTimer::Stop();


	if (_mysqlDriver)
	{

		delete _mysqlDriver;
		_mysqlDriver = 0;

	}
}

const bool Server::Initialize()
{
	_started = false;

	std::cout << ">Server Starting...------------------------------------------------------------------------------\n\n\n";
	//Server timer----------------------------------------------------------
	if (!ServerTimer::Start())
	{
		std::cout << ">ServerTimer failed to start.\n\n";
		return false;
	}
	std::cout << ">ServerTimer started." << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n";

	//WSAStartup----------------------------------------------------------
	if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
		return false;

	//Server Config----------------------------------------------------------
	_serverConfig = LoadConfigFromFile("Config\\server.txt");
	if (!_serverConfig.isValid)
	{
		std::cout << ">Failed to load config file. [Config//server.txt]\n\n";
		return false;
	}
	std::cout << ">Loaded config file. [Config//server.txt]" << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n";

	//Mysql driver----------------------------------------------------------
	_mysqlDriver = new MySqlDriver();
	if (!_mysqlDriver->initDriver(_serverConfig.mysqlHostAddress.c_str(), _serverConfig.mysqlUsername.c_str(), _serverConfig.mysqlPassword.c_str(), _serverConfig.database.c_str()))
	{
		std::cout << "Failed to start MySqlDriver on[" << _serverConfig.mysqlHostAddress << "] user:[" << _serverConfig.mysqlUsername << "] pass[" << _serverConfig.mysqlPassword << "] db[" << _serverConfig.database << "]\n\n";
		return false;
	}
	std::cout << ">Succesfully started MySqlDriver on[" << _serverConfig.mysqlHostAddress << "] user:[" << _serverConfig.mysqlUsername << "] pass[" << _serverConfig.mysqlPassword << "] db[" << _serverConfig.database << "]" << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n";


	//tests

	std::cout << "\n>Loading Data...\n\n";

	//Effects service----------------------------------------------------------
	if (!EffectService::LoadEffects())
	{
		std::cout << ">Failed to load effects.\n\n";
		return false;
	}
	std::cout << ">Effects Service Loaded [" << EffectService::GetCount() << "] effects." << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n";

	//Abnormalities service----------------------------------------------------------
	if (!AbnormalitiesService::LoadAbnormalities())
	{
		std::cout << ">Failed to load abnormalities.\n\n";
		return false;
	}
	std::cout << ">Abnormalities Service Loaded [" << AbnormalitiesService::GetCount() << "] abnormalities." << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n";

	//Inventory service----------------------------------------------------------
	if (!InventoryService::LoadItems())
	{
		std::cout << ">InventoryService failed to load!\n\n";
		return false;
	}
	std::cout << ">Inventory Service loaded[" << InventoryService::_items.size() << "] items." << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n";

	//Skills service----------------------------------------------------------
	if (!SkillService::LoadSkills())
	{
		std::cout << ">SkillService failed to load!\n\n";
		return false;
	}
	std::cout << ">Skill Service loaded[" << SkillService::_skills.size() << "] skills." << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n";

	//Player service----------------------------------------------------------
	if (PlayerService::InitializePlayerService(_mysqlDriver) < 0)
	{

		std::cout << ">PlayerService failed to initialize!\n\n";
		return false;

	}
	std::cout << ">Player Service initialized! Loaded[" << PlayerService::GetAccountCount() << "] accounts!" << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n";

	OpCodes::Add(new RExit());
	OpCodes::Add(new SCheckVersion());
	OpCodes::Add(new SLoginArbiter());
	OpCodes::Add(new SSetVisibleRange());
	OpCodes::Add(new RGetPlayerList());
	OpCodes::Add(new RHardwareInfo());
	OpCodes::Add(new RReqVipSystemInfo());
	OpCodes::Add(new RCanCreatePlayer());
	OpCodes::Add(new RStrEvaluateList());
	OpCodes::Add(new RCheckUsername());
	OpCodes::Add(new RCreatePlayer());
	OpCodes::Add(new RSelectPlayer());
	OpCodes::Add(new RSavePlayerAccountSettings());
	OpCodes::Add(new RMovePlayer());
	OpCodes::Add(new RGetPlayerGuildLogo());
	OpCodes::Add(new RDeletePlayer());
	OpCodes::Add(new RChangeUserLobbySlotId());
	OpCodes::Add(new RCancelDeletePlayer());
	OpCodes::Add(new RLoadTopoFin());
	OpCodes::Add(new RTradeBrokerHighestItemLevel());
	OpCodes::Add(new RUnionSummary());
	OpCodes::Add(new RServerTime());
	OpCodes::Add(new RUpdateContentsPlayTime());
	OpCodes::Add(new RSimpleTipRepeatCheck());
	OpCodes::Add(new RSaveUserSettings());
	OpCodes::Add(new RRequestInstoreProductList());
	OpCodes::Add(new RGuardPkPolicy());
	OpCodes::Add(new RDungeonCoolTimeList());
	OpCodes::Add(new RRequestUserItemLevelInfo());
	OpCodes::Add(new RDungeonClearCountList());
	OpCodes::Add(new RNpcGuildList());
	OpCodes::Add(new RRaignInfo());
	OpCodes::Add(new RVisitNewSection());
	OpCodes::Add(new REventGuide());
	OpCodes::Add(new RShowInven());
	OpCodes::Add(new RSkillLearnRequest());
	OpCodes::Add(new RSkillLearnList());
	OpCodes::Add(new RStartSkill());
	OpCodes::Add(new RCancelSkill());
	OpCodes::Add(new RNotifyLocationInAction());
	OpCodes::Add(new RRequestGameStatPing());


	std::cout << ">Initialized [" << OpCodes::Count() << "] OpCodes Resolutions. Revision[4605]" << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n";

	std::cout << ">Data Loaded.\n\n\n";
	std::cout << ">Initializing Systems...\n\n";

	if (!WorldSystem::InitializeWorldSystem("data\\areas.bin", "data\\creatures.bin", "data\\objects.bin"))
	{
		std::cout << ">Failed to initialize WorldSystem!." << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n";
		return false;
	}
	std::cout << ">WorldSystem Initialized!." << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n";

	BroadcastSystem::InitBroadcastService(1);
	std::cout << ">BroadcastSystem Initialized!." << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n";

	std::cout << ">Systems Initialized." << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n\n";


	_listener = new Listener(_serverConfig.ip.c_str(), _serverConfig.port, _serverConfig.maxClients);
	if (!_listener->Initialize())
	{
		std::cout << ">Listener initialized on IP [" << _serverConfig.ip << "] PORT[" << _serverConfig.port << "]...\n\n";
		return false;
	}
	std::cout << ">Listener initialized on IP [" << _serverConfig.ip << "] PORT[" << _serverConfig.port << "]..." << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n";

	std::cout << ">Server Initialized. Ready to start!" << "   secods[" << ServerTimer::GetTime().totalTime << "]\n\n\n\n\n\n";
	return true;
}

const bool Server::Run()
{
	if (_started)
		return false;

	float t1 = ServerTimer::GetTime().totalTime;
	_listener->Start(this);
	WorldSystem::StartSystem();
	std::cout << ">--------Wrold systems started.\n\n";

	std::cout << ">Server Runnging..." << "   secods[" << ServerTimer::GetTime().totalTime - t1 << "]\n\n";
	_started = true;
	return true;
}


const bool Server::Shutdown()
{
	if (!_started)
		return false;
	WorldSystem::ShutdownSystem();

	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i])
		{
			_clients[i]->Close();
		}
	}
	_listener->Stop();
	_started = false;
	return true;
}

void Server::NewConnection(Client * connection)
{
	_serverMutex.lock();

	if ((_connectedClients + 1) > _serverConfig.maxClients)
	{
		_listener->Stop();
		std::cout << "Stopped listening for clients! [Max clients reached]\n\n";
		connection->Close();

		delete connection;
		connection = 0;

		return;
	}

	connection->SetId(_connectedClients);
	_clients.push_back(connection);
	_connectedClients++;
	_serverMutex.unlock();
}

void Server::EndConnection(Client * c)
{
	std::lock_guard<std::mutex> _lock(_serverMutex);


	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i] && _clients[i] == c)
		{

			_clients[i]->Close();

			delete _clients[i];
			_clients[i] = 0;

			break;
		}
	}


}




Server::Config Server::LoadConfigFromFile(const char * file)
{
	Config out;
	ZeroMemory(&out, sizeof out);


	std::fstream f;
	f.open(file, std::ios::in);

	if (f.is_open())
	{
		std::string line;

		std::getline(f, line);
		out.ip = line;

		std::getline(f, line);
		out.port = atoi(line.c_str());

		std::getline(f, line);
		out.maxClients = atoi(line.c_str());

		std::getline(f, line);
		out.mysqlHostAddress = line.c_str();

		std::getline(f, line);
		out.mysqlUsername = line.c_str();

		std::getline(f, line);
		out.mysqlPassword = line.c_str();

		std::getline(f, line);
		out.database = line.c_str();

		out.isValid = true;
	}

	return out;
}
