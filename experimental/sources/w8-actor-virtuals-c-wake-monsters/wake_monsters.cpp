#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed wakeMonsters__8creaturev at 0x113790, 32 bytes. It passes the address
// of the file-local wakeMonster__FP10GameObject (0x113c90, 48 bytes) to
// objectIterateRunLists, whose body is not defined here.
// Observed wakeMonster: when bit 0x20 of the word at object+12 is set and bit
// 0x40 is clear, bit 0x10 of the byte at object+570 is cleared; otherwise nothing
// is written. Unknown: what the three bits mean. Their pairing with
// sleepMonsters (0x113770) is only inferred from the matching callback shape.
extern "C" void objectIterateRunLists__FPFP10GameObject_v(void (*callback)(void*));

namespace {
void wake_monster(void* object) {
    using namespace w8c;
    const Address base = address(object);
    const unsigned flags = read_word(base + 12u);
    if ((flags & 0x20u) == 0u || (flags & 0x40u) != 0u) return;
    write_u8(base + 570u, read_u8(base + 570u) & 0xefu);
}
}

extern "C" void wakeMonsters__8creaturev() {
    objectIterateRunLists__FPFP10GameObject_v(wake_monster);
}
