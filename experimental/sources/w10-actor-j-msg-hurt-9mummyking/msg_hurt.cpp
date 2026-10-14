// Observed msg_hurt__9MummyKingfP10GameObjecti at 0x0023bf28, 120 bytes. When bit 26 (0x04000000) of the
// 64-bit word at object+160 is set, nothing happens. Otherwise the Creature hurt routine runs with the
// incoming arguments unchanged, then the ten bytes "MummyKing" plus terminator are copied to the start of
// bossMeterName (0x0034b988) and the float at +560 is truncated into bossMeterCurrent (0x00483f48).
// What the bit means is unknown. Nothing here ran on the EE.
#include "../w10-actor-j-common/access.hpp"

extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* attacker, int value);

void msg_hurt__9MummyKingfP10GameObjecti(void* self, float amount, void* attacker, int value) {
    using namespace w10j;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if ((dword(o, 160) & 0x04000000ull) != 0ull) return;
    msg_hurt__8CreaturefP10GameObjecti(self, amount, attacker, value);
    __builtin_memcpy(bossMeterName, "MummyKing", 10);
    bossMeterCurrent = static_cast<int>(real(o, 560));
}
}
