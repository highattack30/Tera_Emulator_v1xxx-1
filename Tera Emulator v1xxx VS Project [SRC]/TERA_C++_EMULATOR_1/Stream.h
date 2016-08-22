////////////////////////////////////////////////////////////
/////////////////Writed by Balan Narcis/////////////////////
////////Created to be used with Tera Emulator v1xxx/////////
////////////////////////////////////////////////////////////

#ifndef STREAM_H
#define STREAM_H
#include "Crypt\data_type.hpp"
#include "Crypt\utils.hpp"
#include "ServerUtils.hpp"

#include <iostream>
#include <vector>
#include <string>

#ifndef STREAM_MAX_SIZE
#define STREAM_MAX_SIZE 0xffff //max size of tera packet
#endif
class Stream
{
public:
	Stream();
	Stream(unsigned int size);
	Stream(byte* data, unsigned int size);
	Stream(std::vector<byte>& vec);
	~Stream();

private:
	void Resize(unsigned int size);
public:
	void Write(byte* data, unsigned int size);
	void WriteHexString(std::string data);
	void WriteString(std::string s, bool reverse = false);
	void WriteByte(byte val);
	void WriteFloat(float val);
	void WriteDouble(double val);
	void WriteInt16(short val, bool swapped = false);
	void WriteInt32(int val, bool swapped = false);
	void WriteInt64(int64_t value);
	void WritePos(short s, short offset = 0);
	void Read(byte* out_buffer, unsigned int size);
	short ReadInt16();
	int  ReadInt32();
	float ReadFloat();
	byte ReadByte();

	std::string ReadString();
	std::string ReadReceivedString();
	std::string ReadASCIIString();

	void Clear();
	unsigned int SetEnd();
	unsigned int SetFront();
	void MoveForward(unsigned int val);
	void MoveBackwards(unsigned int val);

	byte* _raw;
	unsigned int _size;
	unsigned int _pos;
	bool _flag1;

};

#endif



