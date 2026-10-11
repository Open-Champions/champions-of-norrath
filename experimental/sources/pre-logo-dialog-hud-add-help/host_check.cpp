// Host contract for hudAddHelpMessage. Not an EE plan input.
#include "hud_add_help.cpp"

#include <cstdio>
#include <cstdint>
#include <cstring>

extern "C" unsigned char netNetworkMode = 0;
extern "C" int nmPlayersAlive = 0;
extern "C" int playersAlive[4] = {};
extern "C" signed char helpMessageOn = 0;
extern "C" unsigned short* title = nullptr;
extern "C" unsigned short* body = nullptr;
extern "C" int controlPaused = 0;

static int local_calls;
static int local_args[8];
static int busy_calls;
static int busy_player;
static int busy_flag;
static const char* lump_name;
static const char* resource_name;
static void* played_header;
static void* played_object;
static unsigned played_bits;
static int played_result = 44;
static int unpausable;
static int flares;
static int flare_kind;

extern "C" int netPlayerIsLocal__Fi(int player) {
    if (local_calls < 8) local_args[local_calls] = player;
    ++local_calls;
    return player == 4 || player == 7 || player == 0x180 ? 1 : 0;
}

extern "C" void msgHudSetPlayerBusy__Fcc(int player, int busy) {
    ++busy_calls;
    busy_player = player;
    busy_flag = busy;
}

extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    lump_name = lump;
    resource_name = name;
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x51u));
}

extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume) {
    played_header = header;
    played_object = object;
    std::memcpy(&played_bits, &volume, 4);
    return played_result;
}

extern "C" void soundMakeSequenceUnpausable__Fi(int sequence) {
    unpausable = sequence;
}

extern "C" void coreInterfaceFlare__Fi(int kind) {
    ++flares;
    flare_kind = kind;
}

static int same(const char* text, const char* want) {
    return std::strcmp(text, want) == 0;
}

int main() {
    unsigned short first[2] = {1, 0};
    unsigned short second[2] = {2, 0};
    netNetworkMode = 0;
    local_calls = 0;
    hudAddHelpMessage__FiPUsT1(3, first, second);
    if (local_calls != 1 || local_args[0] != 3) return 1;
    if (helpMessageOn != 0 || flares != 0) return 2;

    local_calls = 0;
    busy_calls = 0;
    flares = 0;
    hudAddHelpMessage__FiPUsT1(-1, first, second);
    if (local_calls != 0 || busy_calls != 0 || flares != 1 || flare_kind != 0) return 3;
    if (!same(lump_name, "fx.lmp") || !same(resource_name, "butn_message.anm")) return 4;
    if (played_header != reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x51u)) || played_object != nullptr) return 5;
    if (played_bits != 0x3f800000u || unpausable != 44) return 6;
    if (helpMessageOn != 1 || title != first || body != second || controlPaused != 90) return 7;

    netNetworkMode = 2;
    nmPlayersAlive = 3;
    playersAlive[0] = 1;
    playersAlive[1] = 0x180;
    playersAlive[2] = 4;
    helpMessageOn = 0;
    flares = 0;
    busy_calls = 0;
    local_calls = 0;
    hudAddHelpMessage__FiPUsT1(-1, first, second);
    if (busy_calls != 1 || busy_player != static_cast<signed char>(0x80) || busy_flag != 1) return 8;
    if (local_args[0] != 1 || local_args[1] != 0x180) return 9;
    if (flares != 1 || helpMessageOn != 1) return 10;

    nmPlayersAlive = 1;
    playersAlive[0] = 5;
    helpMessageOn = 0;
    flares = 0;
    busy_calls = 0;
    hudAddHelpMessage__FiPUsT1(7, first, second);
    if (busy_calls != 0 || flares != 0 || helpMessageOn != 0) return 11;
    return 0;
}
