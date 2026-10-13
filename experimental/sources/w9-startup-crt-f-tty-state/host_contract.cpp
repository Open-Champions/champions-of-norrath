// Host check only; not an EE build input.
// Build: EE compiler with the authored_cpp flags plus -DSYNTAX_ONLY -fsyntax-only (layout asserts)
//        c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT -o /tmp/c host_contract.cpp tty_state.cpp && /tmp/c
// The 4-byte-pointer asserts only apply when the compiler's pointers are 4 bytes (the EE profile).
#include "tty_state.hpp"

static_assert(sizeof(TtyPacketHeader) == 12, "packet header is 12 bytes");
static_assert(__builtin_offsetof(TtyPacketHeader, length) == 0 && __builtin_offsetof(TtyPacketHeader, zero) == 2 &&
              __builtin_offsetof(TtyPacketHeader, protocol) == 4 && __builtin_offsetof(TtyPacketHeader, byte6) == 6 &&
              __builtin_offsetof(TtyPacketHeader, byte7) == 7 && __builtin_offsetof(TtyPacketHeader, word8) == 8,
              "packet header offsets");
static_assert(sizeof(void*) != 4 || sizeof(TtyQueue) == 16, "queue header is 16 bytes");
static_assert(sizeof(void*) != 4 || sizeof(TtyInfo) == 48, "tinfo is 48 bytes");
static_assert(sizeof(void*) != 4 || (__builtin_offsetof(TtyInfo, socket) == 0 && __builtin_offsetof(TtyInfo, length) == 4 &&
              __builtin_offsetof(TtyInfo, received) == 8 && __builtin_offsetof(TtyInfo, busy) == 12 &&
              __builtin_offsetof(TtyInfo, sendCursor) == 16 && __builtin_offsetof(TtyInfo, receiveBuffer) == 20 &&
              __builtin_offsetof(TtyInfo, queue) == 24), "tinfo offsets");
static_assert(sizeof(void*) != 4 || (__builtin_offsetof(TtyQueue, size) == 0 && __builtin_offsetof(TtyQueue, count) == 4 &&
              __builtin_offsetof(TtyQueue, readPtr) == 8 && __builtin_offsetof(TtyQueue, writePtr) == 12), "queue offsets");

#ifndef SYNTAX_ONLY
int main() {
    // the three objects start out zero and wbuf covers 12 header bytes plus 256 data bytes
    if (sizeof(wbuf) < 12 + 256) return 1;
    for (unsigned char byte : wbuf) if (byte != 0) return 2;
    for (unsigned i = 0; i < 256; ++i) if (rbuf[i] != 0) return 3;
    const unsigned char* raw = reinterpret_cast<const unsigned char*>(&tinfo);
    for (unsigned i = 0; i < sizeof tinfo; ++i) if (raw[i] != 0) return 4;
    return 0;
}
#endif
