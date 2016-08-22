#include "SQLStream.h"
#include <stdio.h>
#include <string.h>
#include <string>


SQLStream::SQLStream()
{
}


SQLStream::~SQLStream()
{
}

void SQLStream::WriteString(const char * s)
{
	_raw += s;
	_size = _raw.size();
}

void SQLStream::Write(const char * data, unsigned int size)
{
	_raw.insert(_raw.size(), data, size);
	_size = _raw.size();
}

void SQLStream::WriteAndQuoteString(const char * s)
{
	WriteString("'");
	WriteString((char*)s);
	WriteString("'");
}
void SQLStream::EscapeString()
{
	for (size_t i = 0; i < _size; i++)
	{
		if (_raw[i] == 0)
			_raw[i] = '\\0';
	}

	_raw[_size] = 0;
}

void SQLStream::UndoEscape()
{
	for (size_t i = 0; i < _size; i++)
	{
		if (_raw[i] == '0')
			memset(&_raw[i], 0, 1);
	}
}

const char* SQLStream::EscapeData(const char * _data, unsigned int s)
{
	char* data = new char[s];
	memcpy_s((void*)data, s, (const void*)_data, s);
	for (size_t i = 0; i < s; i++)
	{
		if (data[i] == 0 || data[i] == '\n' || data[i] == '\0')
			data[i] = '\\0';
	}
	return data;
}

const char* SQLStream::UndoEscapeData(const char * _data, unsigned int s)
{
	char* data = new char[s];
	memcpy_s((void*)data, s, (const void*)_data, s);
	for (size_t i = 0; i < s; i++)
	{
		if (data[i] == '0')
			memset(&data[i], 0, 1);
	}
	return data;
}



void SQLStream::Clear()
{
	_raw.clear();
	_size = 0;
}

void SQLStream::operator[](char c)
{
	WriteString(&c);
}

void SQLStream::operator[](int i)
{
	WriteString((char*)std::to_string(i).c_str());
}

void SQLStream::operator[](short s)
{
	WriteString((char*)std::to_string(s).c_str());
}

void SQLStream::operator[](double d)
{
	WriteString((char*)std::to_string(d).c_str());
}

void SQLStream::operator[](float f)
{
	WriteString((char*)std::to_string(f).c_str());
}

void SQLStream::operator[](long long l)
{
	WriteString((char*)std::to_string(l).c_str());
}

SQLStream::operator const char*()
{
	return _raw.c_str();
}

