#include "ServerUtils.hpp"

std::string ServerUtils::HexString(byte * data, unsigned int size)
{
	std::stringstream ss;
	ss << std::hex;
	for (int i = 0; i < size; ++i)
		ss << std::setw(2) << std::setfill('0') << (int)data[i];
	return ss.str();

}

inline unsigned char ServerUtils::HexCharToByteChar(const char * s)
{
	return
		(((s[0] >= 'A' && s[0] <= 'Z') ? (10 + s[0] - 'A') :
		(s[0] >= 'a' && s[0] <= 'z') ? (10 + s[0] - 'a') :
			(s[0] >= '0' && s[0] <= '9') ? (s[0] - '0') : 0) << 4) |
			((s[1] >= 'A' && s[1] <= 'Z') ? (10 + s[1] - 'A') :
		(s[1] >= 'a' && s[1] <= 'z') ? (10 + s[1] - 'a') :
				(s[1] >= '0' && s[1] <= '9') ? (s[1] - '0') : 0);
}

byte * ServerUtils::HexStringToByteArray(std::string str)
{
	if (str.length() % 2 != 0)
		return nullptr;

	byte * out_data = new byte[str.length() / 2];
	char* pos = &str[0];

	for (int count = 0; count < str.length() / 2; count++) {
		sscanf(pos, "%2hhx", &out_data[count]);
		pos += 2;
	}

	return out_data;
}

void ServerUtils::UndoEscapedData(byte * data, unsigned size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (data[i] == '0' || data[i] == 0x48 || data[i] == 0x30)
			data[i] = 0x00;
	}
}

