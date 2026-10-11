// Observed amxSimpleNewWorld__FP5__amxPi at 0x00139430, 240 bytes. Cell 1
// is an AMX string. A nonzero scriptInPostAction byte, a zero
// netNetworkMode byte, or activePlayer == -1 kicks. Otherwise the unsigned
// halfword at player + 23968 + activePlayer * 23984 + 2 is read. Zero stores
// 900 there and kicks. Nonzero and a local player shows textGetString of
// "noZone3" for 180 ticks and does not kick. A kick calls gameNetKick(0),
// stores 0xFF at levelName byte 80, and copies the script name onto
// levelName. The native returns 0. amx_GetAddr's stack slot is zeroed
// first; the original slot was left uninitialized when that call failed.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" unsigned char scriptInPostAction;
extern "C" unsigned char netNetworkMode;
extern "C" int activePlayer;
extern "C" unsigned char player[];
extern "C" char levelName[];
extern "C" int netPlayerIsLocal__Fi(int player);
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" int netGetLocalPlayerNm__Fi(int player);
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int ticks, int player, int unused);
extern "C" void gameNetKick__Fi(int reason);
extern "C" char* strcpy(char* dest, const char* source);

namespace {
unsigned load_half(const unsigned char* bytes) {
    return bytes[0] | (static_cast<unsigned>(bytes[1]) << 8);
}

void store_half(unsigned char* bytes, unsigned value) {
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
}

void kick(char* name) {
    gameNetKick__Fi(0);
    levelName[80] = static_cast<char>(0xff);
    strcpy(levelName, name);
}
}

extern "C" int amxSimpleNewWorld__FP5__amxPi(void* amx, int* params) {
    char name[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    if (scriptInPostAction != 0 || netNetworkMode == 0 || activePlayer == -1) {
        kick(name);
        return 0;
    }
    unsigned char* slot = player + 23968 + activePlayer * 23984;
    if (load_half(slot + 2) == 0) {
        store_half(slot + 2, 900);
        kick(name);
        return 0;
    }
    if (netPlayerIsLocal__Fi(activePlayer)) {
        char key[] = "noZone3";
        hudSetInfoMessage__FPUsiic(textGetString__FPc(key), 180, netGetLocalPlayerNm__Fi(0), 0);
    }
    return 0;
}
