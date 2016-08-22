#include "TeraPacket.hpp"

TeraPacket::TeraPacket()
{
	_size = 0x00;
	_pos = 0x00;
	_raw = 0;

}

TeraPacket::~TeraPacket()
{

	if (_raw)
	{
		delete[] _raw;
		_raw = 0;
	}
	_size = 0;
	_pos = 0;

}

const bool TeraPacket::GetPacket(SOCKET sock, Crypt::Session * session)
{
	_recvMutex.lock();

	int read = 0;
	int len = 0;
	Resize(4);
	read = recv(sock, (char*)_raw, 4, 0);

	if (read == SOCKET_ERROR || read == 0)
	{
		shutdown(sock, SD_BOTH);
		closesocket(sock);
		read = -1;
		_recvMutex.unlock();
		return false;
	}
	len = read;
	if (sock == INVALID_SOCKET)
		return false;

	session->Decrypt(_raw, 4);

	short Packetsize = ((_raw[1] << 8) | _raw[0]);
	_opCode[0] = _raw[3];
	_opCode[1] = _raw[2];


	Resize(Packetsize - 4);

	while (_size > len && sock != INVALID_SOCKET && read >= 0)
	{
		read = recv(sock, (char*)&_raw[len], _size - len, 0);

		if (read == SOCKET_ERROR)
		{
			if (!sock == INVALID_SOCKET)
			{
				_recvMutex.unlock();
			}

			delete[] _raw;
			_raw = 0;

			shutdown(sock, SD_BOTH);
			closesocket(sock);
			return false;
		}
		len += read;
	}
	if (sock == INVALID_SOCKET)
		return false;

	if (_size > 4)
		session->Decrypt(&_raw[4], _size - 4);

	if (sock != INVALID_SOCKET)
		_recvMutex.unlock();

	return true;
}

void TeraPacket::Resize(unsigned short size)
{
	if ((_size + size) > 0xffff)
		return;
	if (!_raw)
	{
		_raw = new byte[size];
		_size = size;
		return;
	}

	short newSize = _size + size;;
	byte * newRaw = (byte*)realloc(_raw, (unsigned int)newSize);
	if (newRaw == 0)
		return;

	memset((void*)&newRaw[_size], 0, size);

	_size = newSize;
	_raw = newRaw;
}

void TeraPacket::Reset()
{
	if (_raw)
	{
		delete[] _raw;
		_raw = 0;
	}
	_size = 0;
	_pos = 0;
}

const bool TeraPacket::IsValidPacket()
{
	if (!_raw || (_size < 4))
		return false;

	return true;
}

