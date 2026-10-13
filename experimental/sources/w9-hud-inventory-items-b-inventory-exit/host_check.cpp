#include <cstring>
extern "C" {
unsigned char inventory[464];
unsigned char statsScreen[232];
unsigned char player[23984 * 4 + 512];
int buySellVertSpacing;
int invMsgHandle;
signed char netNetworkMode;
extern const unsigned char norrath_inventory_ability_sound_lump_view[8];
extern const unsigned char norrath_inventory_exit_sound_resource_view[21];
const unsigned char norrath_inventory_ability_sound_lump_view[8] = {'f','x','.','l','m','p',0,0};
const unsigned char norrath_inventory_exit_sound_resource_view[21] = {'e',0};
void inventoryExit__Fiii(int, int, int);
}

static int log_[64], n;
static int unpackSetsQ = -1;
static int local;
static unsigned char* screenSeen;
static int sendCopy[3];
static int sentHandle, sentLen;
static void* soundHandle;
static float soundRate;
static void* soundObject;
static int seen112;

extern "C" void resetMenu__FP12screenStruct(void* s) { log_[n++] = 1; screenSeen = (unsigned char*)s; std::memset(s, 0, 0); }
extern "C" void inventoryUnpack__Fiib(int a, int p, bool z) {
    log_[n++] = 2; log_[n++] = a; log_[n++] = p; log_[n++] = z;
    // the +24/+36/+76 stores must already be done at this point
    seen112 = *reinterpret_cast<int*>(screenSeen + 24) * 1000 + (*reinterpret_cast<int*>(screenSeen + 76) == 0);
    if (unpackSetsQ >= 0) *reinterpret_cast<int*>(screenSeen + 52) = unpackSetsQ;
}
extern "C" int netPlayerIsLocal__Fi(int q) { log_[n++] = 3; log_[n++] = q; return local; }
extern "C" void* lumpFindResource__FPcT0(char* l, char* r) { log_[n++] = 4; if (l != (char*)norrath_inventory_ability_sound_lump_view || r != (char*)norrath_inventory_exit_sound_resource_view) log_[n++] = -99; return (void*)0x1234; }
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float r) { log_[n++] = 5; soundHandle = h; soundObject = o; soundRate = r; return 0; }
extern "C" void netSendMessage__FiPUci(int h, unsigned char* m, int len) { log_[n++] = 6; sentHandle = h; sentLen = len; for (int i = 0; i < 3; ++i) sendCopy[i] = m[i]; }

static void reset() { n = 0; std::memset(inventory, 0, sizeof inventory); std::memset(statsScreen, 0, sizeof statsScreen); std::memset(player, 0, sizeof player); unpackSetsQ = -1; }
static int& word(int p, int off) { return *reinterpret_cast<int*>(inventory + p * 116 + off); }

int main() {
    buySellVertSpacing = 7; invMsgHandle = 42;

    // without announce: only reset, field copies, unpack
    reset(); word(1, 112) = 6; word(1, 76) = 99; local = 1;
    inventoryExit__Fiii(1, 55, 0);
    if (n != 5 || log_[0] != 1 || log_[1] != 2 || log_[2] != 55 || log_[3] != 1 || log_[4] != 0) return 1;
    if (screenSeen != inventory + 116) return 2;
    if (word(1, 24) != 6 || word(1, 36) != 42 || word(1, 76) != 0 || seen112 != 6001) return 3;
    if (word(1, 80) != 0 || statsScreen[16] || statsScreen[116 + 16] || sentLen == 3) return 4;

    // announce, local, offline: sound, message, flags
    reset(); netNetworkMode = 0; local = 1; unpackSetsQ = 2; word(0, 112) = 3;
    player[2 * 23984 + 89] = 0xee; player[2 * 23984 + 242] = 0x77;
    inventoryExit__Fiii(0, 8, 1);
    // call order: reset, unpack(3 values), isLocal(q read after the unpack), find, play, send
    int expect[] = {1, 2, 8, 0, 0, 3, 2, 4, 5, 6};
    if (n != 10) return 5;
    for (int i = 0; i < 10; ++i) if (log_[i] != expect[i]) return 6;
    if (soundHandle != (void*)0x1234 || soundObject != nullptr || soundRate != 1.0f) return 7;
    if (sentHandle != 42 || sentLen != 3 || sendCopy[0] != 9 || sendCopy[1] != 2 || sendCopy[2] != 0xee) return 8;
    if (word(0, 80) != 5 || player[2 * 23984 + 241] != 2 || player[2 * 23984 + 242] != 0) return 9;
    if (statsScreen[16] != 1 || statsScreen[116 + 16] != 0) return 10;
    if (word(0, 36) != 21) return 11;

    // announce, not local, screen 1, offline: no sound, statsScreen[1] set
    reset(); netNetworkMode = 0; local = 0; unpackSetsQ = 1;
    inventoryExit__Fiii(1, 0, 1);
    if (n != 8) return 12;
    if (statsScreen[116 + 16] != 1 || statsScreen[16] != 0) return 13;
    if (player[23984 + 241] != 1) return 14;

    // network mode: statsScreen[0] regardless of the screen index
    reset(); netNetworkMode = 1; local = 0; unpackSetsQ = 0;
    inventoryExit__Fiii(1, 0, 1);
    if (statsScreen[16] != 1 || statsScreen[116 + 16] != 0) return 15;
    return 0;
}
