// Observed isEnemy__6PlayerP10GameObject at 0x001aba90, 88 bytes. other is the second argument; its
// flag word is at +12 and its doubleword at +160. The result is 1 when all of these hold, else 0:
//   flag word has bit 5 (0x20) set;
//   flag word bits 3 (0x8), 6 (0x40) and 24 (0x1000000) are all clear;
//   the signed halfword at other+792 is zero;
//   the doubleword at +160 has bit 26 (0x4000000) and bit 11 (0x800) clear.
// The receiver is not read. Inferred: the result answers "is other something a player can attack";
// that is a reading of the name only. Unknown: what the individual bits mean. Nothing here ran on the EE.
extern "C" int isEnemy__6PlayerP10GameObject(void*, void* other) {
    const unsigned char* o = static_cast<const unsigned char*>(other);
    const unsigned flags = *reinterpret_cast<const unsigned*>(o + 12);
    if (!(flags & 0x20u)) return 0;
    if (flags & 0x8u) return 0;
    if (flags & 0x40u) return 0;
    if (flags & 0x1000000u) return 0;
    if (*reinterpret_cast<const short*>(o + 792) != 0) return 0;
    const unsigned long long wide = *reinterpret_cast<const unsigned long long*>(o + 160);
    if (wide & 0x4000000ull) return 0;
    return (wide & 0x800ull) == 0;
}
