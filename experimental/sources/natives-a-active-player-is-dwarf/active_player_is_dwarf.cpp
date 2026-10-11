// amxActivePlayerIsDwarf is the 32-byte native at 0x0013bcd0. It reads the
// word at byte 84 of player record activePlayer. Records are the 95936-byte
// player object at 0x00408600, 23984 bytes apart. The native returns 1 when
// that word is zero.
extern "C" int activePlayer;
extern "C" unsigned char player[];
static unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) |
           (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) |
           (static_cast<unsigned>(address[3]) << 24);
}
extern "C" int amxActivePlayerIsDwarf__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    const unsigned index = static_cast<unsigned>(activePlayer);
    return load_u32(player + index * 23984u + 84u) == 0u ? 1 : 0;
}
