// Observed gameNetKick at 0x001d0900, 224 bytes.
// A zero netNetworkMode byte returns before any call. Otherwise the calls
// are netGetLocalPlayerNm(0), msgHudSetPlayerBusy with the sign-extended low
// byte of that result and 0, debugClearStartMenu, invShutDown(0), hudKick,
// MC_Kick, and shopKick. A nonzero argument returns after those calls. A zero
// argument walks playersAlive while the index is below nmPlayersAlive.
// nmPlayersAlive is loaded again only after Kick. The playersAlive word is
// the record index. The word at player byte 21872 is the entity, and the
// word at entity byte 1328 is the argument of Kick. A zero vehicle word
// skips Kick and keeps the previously loaded bound.
extern "C" signed char netNetworkMode;
extern "C" int nmPlayersAlive;
extern "C" int playersAlive[];
extern "C" unsigned char player[];
extern "C" int netGetLocalPlayerNm__Fi(int player);
extern "C" void msgHudSetPlayerBusy__Fcc(int player, int second);
extern "C" void debugClearStartMenu__Fv();
extern "C" void invShutDown__Fi(int value);
extern "C" void hudKick__Fv();
extern "C" void MC_Kick__Fv();
extern "C" void shopKick__Fv();
extern "C" void Kick__7Vehicle(unsigned char* vehicle);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_game_loop_host_base;
#endif

namespace {
unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

unsigned char* pointer_at(const unsigned char* address) {
    unsigned word = load_u32(address);
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_game_loop_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}

bool vehicle_present(unsigned char* vehicle) {
#if defined(NORRATH_HOST_CONTRACT)
    return vehicle != norrath_game_loop_host_base;
#else
    return vehicle != nullptr;
#endif
}
}

extern "C" void gameNetKick__Fi(int value) {
    if (netNetworkMode == 0) return;
    int local = netGetLocalPlayerNm__Fi(0);
    int low = static_cast<int>(static_cast<signed char>(local & 0xff));
    msgHudSetPlayerBusy__Fcc(low, 0);
    debugClearStartMenu__Fv();
    invShutDown__Fi(0);
    hudKick__Fv();
    MC_Kick__Fv();
    shopKick__Fv();
    if (value != 0) return;
    int bound = nmPlayersAlive;
    int index = 0;
    while (index < bound) {
        int slot = playersAlive[index];
        unsigned char* entity = pointer_at(player + static_cast<unsigned>(slot) * 23984u + 21872u);
        unsigned char* vehicle = pointer_at(entity + 1328);
        if (vehicle_present(vehicle)) {
            Kick__7Vehicle(vehicle);
            bound = nmPlayersAlive;
        }
        ++index;
    }
}
