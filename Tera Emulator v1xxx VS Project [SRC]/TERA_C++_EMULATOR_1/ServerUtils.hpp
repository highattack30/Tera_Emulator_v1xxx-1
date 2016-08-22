#ifndef SERVERUTILS_H
#define SERVERUTILS_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdio.h>

#include "Crypt\data_type.hpp"

class ServerUtils
{
public:

	static std::string HexString(byte* data, unsigned int size);
	static unsigned char HexCharToByteChar(const char *s);
	static byte* HexStringToByteArray(std::string str);
	static void UndoEscapedData(byte* data, unsigned size);
};
#endif
