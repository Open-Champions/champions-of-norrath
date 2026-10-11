// amxCheckPlayerGold is the 60-byte native at 0x0013bbe8. It sums the word
// at byte 232 of each of the first nmPlayers records in player. The stride
// is 23984. A non-positive count returns zero. The sum wraps at 32 bits.
extern "C" int nmPlayers;
extern "C" unsigned char player[];
static unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) |
           (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) |
           (static_cast<unsigned>(address[3]) << 24);
}
extern "C" int amxCheckPlayerGold__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    int count = nmPlayers;
    if (count <= 0) return 0;
    unsigned total = 0u;
    unsigned char* gold = player + 232;
    do {
        unsigned value = load_u32(gold);
        gold += 23984;
        count -= 1;
        total += value;
    } while (count != 0);
    return static_cast<int>(total);
}
