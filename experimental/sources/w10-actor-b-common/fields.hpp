#pragma once
// Field access for the actor methods of this package. Object fields stay at their
// observed byte offsets; scalar fields are read and written through may_alias types.
// Words that hold pointers in the 32-bit target image go through rtti_c::pointer(),
// which maps them back on a host check (define NORRATH_RTTI_C_HOST there).
#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
namespace actor_b {
typedef unsigned Word __attribute__((may_alias));
typedef int SWord __attribute__((may_alias));
typedef unsigned short Half __attribute__((may_alias));
typedef short SHalf __attribute__((may_alias));
typedef float Single __attribute__((may_alias));
typedef unsigned char Byte __attribute__((may_alias));
typedef signed char SByte __attribute__((may_alias));
template <typename T>
__attribute__((always_inline)) inline T& field(void* object, unsigned offset) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset);
}
// The word at object+offset used as a pointer.
__attribute__((always_inline)) inline void* link(void* object, unsigned offset) {
    return rtti_c::pointer(field<Word>(object, offset));
}
}
