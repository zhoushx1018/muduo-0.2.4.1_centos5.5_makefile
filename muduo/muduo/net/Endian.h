// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
// Modify: ENDIAN functions  are  nonstandard

// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_ENDIAN_H
#define MUDUO_NET_ENDIAN_H

#include <stdint.h>
#include <arpa/inet.h>

namespace muduo
{
namespace net
{
namespace sockets
{
// the inline assembler code makes type blur,
// so we disable warnings for a while.
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"
inline uint64_t hostToNetwork64(uint64_t host64)
{
    union { uint32_t lv[2]; uint64_t llv; } u;
    u.lv[0] = htonl(static_cast<uint32_t>(host64 >> 32));
    u.lv[1] = htonl(static_cast<uint32_t>(host64 & 0xffffffff));
    return u.llv;
}

inline uint32_t hostToNetwork32(uint32_t host32)
{
    return htonl(host32);
}

inline uint16_t hostToNetwork16(uint16_t host16)
{
    return htons(host16);
}

inline uint64_t networkToHost64(uint64_t net64)
{
    union { uint32_t lv[2]; uint64_t llv; } u;
    u.lv[0] = ntohl(static_cast<uint32_t>(net64 >> 32));
    u.lv[1] = ntohl(static_cast<uint32_t>(net64 & 0xffffffff));
    return u.llv;
}

inline uint32_t networkToHost32(uint32_t net32)
{
    return ntohl(net32);
}

inline uint16_t networkToHost16(uint16_t net16)
{
    return ntohs(net16);
}
#pragma GCC diagnostic error "-Wconversion"
#pragma GCC diagnostic error "-Wold-style-cast"

}
}
}

#endif  // MUDUO_NET_ENDIAN_H
