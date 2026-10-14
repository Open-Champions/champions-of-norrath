// Observed amxPlayerHasItem at 0x0013ba30, 120 bytes. It sums
// queryInventoryItem over indexes 0 .. nmPlayers-1. Both the count
// and the name from the first AMX cell are read before each query.
extern "C" unsigned char nmPlayers[4];
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" int queryInventoryItem__FPci(char* name, int index);

static int signed_word(const unsigned char* bytes) {
    const unsigned word = static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24);
    return static_cast<int>(word);
}

extern "C" int amxPlayerHasItem__FP5__amxPi(void* amx, int* params) {
    char text[160];
    void* cells = nullptr;
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(text, cells);
    int total = 0;
    int index = 0;
    int count = signed_word(nmPlayers);
    if (count <= 0) return 0;
    do {
        const int found = queryInventoryItem__FPci(text, index);
        index += 1;
        count = signed_word(nmPlayers);
        total += found;
    } while (index < count);
    return total;
}
