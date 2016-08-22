#include "SLoginArbiter.h"
#include "PlayerService.h"
#include "Account.hpp"
#include "Client.hpp"

SLoginArbiter::SLoginArbiter() :SendPacket(C_LOGIN_ARBITER)
{

}
void SLoginArbiter::Process(OpCode opCode, Stream * stream, Client * caller)
{
	short nameOffset = stream->ReadInt16();
	short ticketOffset = stream->ReadInt16();
	short ticketBytesCount = stream->ReadInt16();/*
	byte* ticket = new byte[ticketBytesCount];
	stream->_pos = ticketOffset;
	stream->Read(ticket, ticketBytesCount);
	a->_ticketSize = ticketBytesCount;
	a->_ticket = ticket;*/

	stream->_pos = 18;
	std::string username = stream->ReadString();

	int result = PlayerService::PerformAccountLogin(username.c_str(), "");

	if (result >= 0)
	{
		
		Account * a = PlayerService::GetAccount(result);
		if (a)
		{
			caller->_account = a;
			stream->Clear();

			
			stream->WriteInt16(0);
			stream->WriteInt16(S_LOADING_SCREEN_CONTROL_INFO);
			stream->WriteByte(0);//unk
			stream->WritePos(0);
			
			caller->Send(stream);
			stream->Clear();

			stream->WriteInt16(0);
			stream->WriteInt16(OpCode::S_REMAIN_PLAY_TIME);
			stream->WriteInt32(6); //unk
			stream->WriteInt32(0); //unk
			stream->WritePos(0);

			caller->Send(stream);
			stream->Clear();


			stream->WriteInt16(0);
			stream->WriteInt16(OpCode::S_LOGIN_ARBITER);
			stream->WriteInt32(1);
			stream->WriteInt32(0);
			stream->WriteInt16(0);
			stream->WriteInt32(6);
			stream->WriteInt32(0);
			stream->WriteByte(0);
			stream->WritePos(0);

			caller->Send(stream);
			stream->Clear();

			stream->WriteInt16(0);
			stream->WriteInt32(OpCode::S_LOGIN_ACCOUNT_INFO);
			stream->WriteInt16(14); // server name offset
			stream->WriteInt32(0x006bfd4e); //unk
			stream->WriteInt32(0);
			stream->WriteString("Planet Gazah");
			stream->WritePos(0);

			caller->Send(stream);
			stream->Clear();

		}
		else
			caller->Close();
	}
	else
		caller->Close();


}