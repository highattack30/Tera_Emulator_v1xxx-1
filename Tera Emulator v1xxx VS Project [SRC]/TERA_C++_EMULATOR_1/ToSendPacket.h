#pragma once


enum BC_TYPE :int
{
	ME = 0,
	VISIBLE_CLIENTS,
	AREA,
	ENEMYE,
	PARTY,
	RAID,
	CHAT
};



class Stream; class Client; 
class ToSendPacket {
	friend class BroadcastSystem;
	Stream * _data;
	Client* _callerClient;
	void** _args;
	BC_TYPE _bcType;
	bool _ordered;

	ToSendPacket(Client* caller, Stream * data, BC_TYPE type,bool ordered, void** args);
	~ToSendPacket();
};