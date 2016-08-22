#ifndef _DATA_TYPE_HPP_
#define _DATA_TYPE_HPP_

#include <cstdint>

#define ulong	uint64_t
#define uint	uint32_t
#define ushort	uint16_t
#define uchar	uint8_t
#define byte	uint8_t
#define Wchar	uint16_t

#ifdef WIN32
	#define socklen_t int
#else
	#define SOCKET int
	#define SOCKET_ERROR -1
	#define INVALID_SOCKET -1
	#define SOCKADDR const struct sockaddr
#endif

#endif // _DATA_TYPE_HPP_