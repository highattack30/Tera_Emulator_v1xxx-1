#pragma once

#include <string>
#include "../Crypt/data_type.hpp"
#define SQL_STREAM_MAX_SIZE 0xffffffff
class SQLStream
{
public:
	SQLStream();
	~SQLStream();

private:

public:
	
	void WriteString(const char* s);
	void Write(const char* data, unsigned int size);
	void WriteAndQuoteString(const char* s);
	void EscapeString();
	void UndoEscape();

	static const char* EscapeData(const char* data, unsigned int s);
	static const char* UndoEscapeData(const char* data, unsigned int s);
	
	void Clear();
	void operator [](char);
	void operator [](int);
	void operator [](short);
	void operator [](double);
	void operator [](float);
	void operator [](long long);

	operator const char*();
	std::string _raw;
	unsigned int _size;
};

