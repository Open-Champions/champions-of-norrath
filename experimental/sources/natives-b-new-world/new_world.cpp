// Observed amxNewWorld at 0x00139350, 220 bytes. A nonzero
// scriptInPostAction byte travels immediately. Otherwise a zero
// netNetworkMode byte, or activePlayer equal to -1, also travels.
// The halfword at player+23970 for that active player is set to 900
// when it is zero, and that travels too. A nonzero halfword shows
// "noZone3" only for a local player and does not travel. Travel is
// gameNetKick(0) and doNewWorld of the first AMX cell.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" signed char scriptInPostAction;
extern "C" signed char netNetworkMode;
extern "C" int activePlayer;
extern "C" unsigned char norrath_player_progression_records_view[];
extern "C" int netPlayerIsLocal__Fi(int player);
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" int netGetLocalPlayerNm__Fi(int code);
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int duration, int player, int color);
extern "C" void gameNetKick__Fi(int code);
extern "C" void doNewWorld__FPc(char* name);

static unsigned short half_at(const unsigned char* bytes) {
    return static_cast<unsigned short>(bytes[0] | (static_cast<unsigned>(bytes[1]) << 8));
}
static void store_half(unsigned char* bytes, unsigned short value) {
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
}

extern "C" int amxNewWorld__FP5__amxPi(void* amx, int* params) {
    char name[160];
    void* cells = nullptr;
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(name, cells);
    if (scriptInPostAction == 0) {
        const int active = activePlayer;
        int blocked = 0;
        if (netNetworkMode != 0 && active != -1) {
            unsigned char* timer = norrath_player_progression_records_view + active * 23984 + 23970;
            if (half_at(timer) == 0) store_half(timer, 900);
            else {
                if (netPlayerIsLocal__Fi(active) != 0) {
                    unsigned short* text = textGetString__FPc(const_cast<char*>("noZone3"));
                    const int local = netGetLocalPlayerNm__Fi(0);
                    hudSetInfoMessage__FPUsiic(text, 180, local, 0);
                }
                blocked = 1;
            }
        }
        if (blocked) return 0;
    }
    gameNetKick__Fi(0);
    doNewWorld__FPc(name);
    return 0;
}
