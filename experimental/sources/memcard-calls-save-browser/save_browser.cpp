// Observed: MC_SaveGameBrowser__Fi at 0x002506b8 is 244 bytes. It scans
// nmPlayers records of the player object. A record counts when the word at
// offset 21872 is nonzero, bit 3 of the word at offset 12 of that object is
// clear, and the float at offset 172 is greater than zero. With no such
// record it returns. It also returns when disableMC is nonzero, asyncMode
// is not -1, or actionCoolOff is nonzero. A nonzero netNetworkMode returns
// unless netPlayerIsLocal of the full argument is nonzero. Otherwise it
// calls pushPause, stores the argument's low byte in whichPad, clears
// ranConfig, ranBrowser, and MC_LastModeSuccessful, calls
// msgHudSetPlayerBusy with the sign-extended low byte and 1, then
// newMode(2, 0).
// Inferred: player records are 23984 bytes. Four of them fill the player
// object. The float and the object word are the same record.
// Unknown: what the float and bit 3 represent.

extern "C" int nmPlayers;
extern "C" unsigned char player[];
extern "C" int disableMC;
extern "C" int asyncMode;
extern "C" int actionCoolOff;
extern "C" signed char netNetworkMode;
extern "C" int netPlayerIsLocal__Fi(int player_index);
extern "C" void pushPause__Fv();
extern "C" signed char whichPad;
extern "C" int ranConfig;
extern "C" int ranBrowser;
extern "C" unsigned char MC_LastModeSuccessful;
extern "C" void msgHudSetPlayerBusy__Fcc(int player_index, int busy);
extern "C" int newMode__Fii(int mode, int argument);

namespace {
unsigned load_word(const unsigned char* at) {
    return static_cast<unsigned>(at[0]) | (static_cast<unsigned>(at[1]) << 8) |
           (static_cast<unsigned>(at[2]) << 16) | (static_cast<unsigned>(at[3]) << 24);
}

float load_float(const unsigned char* at) {
    union {
        unsigned bits;
        float value;
    } pun;
    pun.bits = load_word(at);
    return pun.value;
}

#ifdef NORRATH_HOST_ARENA
extern "C" const unsigned char* norrath_host_objects[];
const unsigned char* object_from(unsigned word) { return norrath_host_objects[word]; }
#else
const unsigned char* object_from(unsigned word) {
    return reinterpret_cast<const unsigned char*>(word);
}
#endif

int low_byte(int value) {
    int low = value & 0xff;
    return low < 128 ? low : low - 256;
}

int living_player() {
    int count = nmPlayers;
    int any = 0;
    if (count > 0) {
        unsigned char* record = player + 172;
        do {
            unsigned word = load_word(record + 21700);
            if (word != 0) {
                unsigned flags = load_word(object_from(word) + 12);
                if ((flags & 8u) == 0 && 0.0f < load_float(record)) any = 1;
            }
            record += 23984;
            count -= 1;
        } while (count != 0);
    }
    return any;
}
}

extern "C" void MC_SaveGameBrowser__Fi(int argument) {
    if (living_player() == 0) return;
    if (disableMC != 0) return;
    if (asyncMode != -1) return;
    if (actionCoolOff != 0) return;
    if (netNetworkMode != 0 && netPlayerIsLocal__Fi(argument) == 0) return;
    pushPause__Fv();
    whichPad = static_cast<signed char>(low_byte(argument));
    ranConfig = 0;
    ranBrowser = 0;
    MC_LastModeSuccessful = 0;
    msgHudSetPlayerBusy__Fcc(low_byte(argument), 1);
    newMode__Fii(2, 0);
}
