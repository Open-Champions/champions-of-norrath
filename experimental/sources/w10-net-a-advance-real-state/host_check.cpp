// Host check for advanceRealState. Not an EE plan input.
// Build: c++ -std=c++17 -Wno-vla-cxx-extension -DNORRATH_W10NA_HOST=1 host_check.cpp ../w10-net-a-anal-compress/anal_compress.cpp
// Pointer-valued words are indexes into a handle table. The compressor is the
// authored analCompress; the decompressor is a stand-in that inverts it for one
// frame, so a nonzero result also proves the bit coding round-trips buttons and yaw.
#define NORRATH_W10NA_HOST 1
#include "advance_real_state.cpp"
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>

static std::vector<void*> handles(1, nullptr);
static unsigned handle_of(void* p) {
    for (unsigned i = 1; i < handles.size(); ++i) if (handles[i] == p) return i;
    handles.push_back(p);
    return static_cast<unsigned>(handles.size() - 1);
}
extern "C" Byte* norrath_w10na_word_to_pointer(unsigned w) { return static_cast<Byte*>(handles[w]); }
extern "C" unsigned norrath_w10na_pointer_to_word(const void* p) { return handle_of(const_cast<void*>(p)); }

static std::string trace;
static void note(const char* s) { trace += s; trace += ' '; }

extern "C" {
signed char netNetworkMode;
int nmPartners, nmPlayers, trueNetFrameNm, frameNm, derivedFrameNm, syncRandState, logChecksum, objectIgnoreFlags;
signed char netTrueStep, netPrediction, netLogDisable, netForceLocalUpdate;
unsigned localPredictedPlayer;
unsigned short netCameraYaw[4];
unsigned char localPlayerPadState[8];
alignas(8) unsigned char partners[4 * 17632 + 64];
alignas(8) unsigned char player[4 * 23984 + 4096];
unsigned char pad[4 * 88];
alignas(8) unsigned char netLocalPad[4 * 88];
unsigned msgHandler[4];
char levelName[16];
alignas(8) unsigned char compLocalCompressor[80], compLocalDecompressor[80];
alignas(8) unsigned char compImmediateCompressor[80], compImmediateDecompressor[80];

static unsigned char last_pad_data[12];
void padProcessData__FP3padP18padUnprocessedDataP12padStateData(void*, void* data, void*) {
    std::memcpy(last_pad_data, data, 12);
    note("pad");
}
void coreGameStep__Fii(int, int) { note(netTrueStep ? "core1" : "core0"); }
void worldPlotRouteProcess__Fi(int) { note(netTrueStep ? "route1" : "route0"); }
void playerDamageHandler__6Player(void*) { note("dmg"); }
void objectAddToGrid__FP10GameObject(void*) { note("grid"); }
static int compares;
void compareStatePackets__FP11statePacketT0i(void*, void*, int partner) { ++compares; note(partner ? "cmp1" : "cmp0"); }
}

// analDecompress stand-in for one frame (count 1): inverts analCompress.
static const short kSteps[9] = {4, 6, 8, 12, 16, 24, 32, 48, 64};
extern "C" void analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(
    void*, void* reader_argument, void* state_argument, unsigned char* out, int, int) {
    BitReader* reader = static_cast<BitReader*>(reader_argument);
    int* state = static_cast<int*>(state_argument);
    unsigned zeros = get(reader, 5);
    int delta = 0;
    if (zeros == 0) {
        int sign = get(reader, 1);
        get(reader, 3);
        delta = sign ? -1 : 1;
    }
    int step = kSteps[state[0]];
    int value = state[1] + delta * step;
    state[1] = value < 0 ? 0 : value > 255 ? 255 : value;
    int index = state[0] + (delta ? 2 : -1);
    state[0] = index < 0 ? 0 : index > 8 ? 8 : index;
    out[0] = static_cast<unsigned char>(state[1]);
}

static void h_msg(unsigned char* p, int n) {
    static char buf[64];
    std::snprintf(buf, sizeof buf, "msg%d:%d", n ? p[0] : -1, n);
    note(buf);
}
static void h_destroy(Byte*, int arg) { note(arg == 3 ? "destroy3" : "destroy?"); }
static void h_damage(Byte*, int a, unsigned flags, float amount) {
    note(a == 0 && flags == 0x20000040u && amount == 1000000.0f ? "damage" : "damage?");
}
alignas(8) static Byte clone_object[256];
static Byte* h_clone(Byte*) { note("clone"); return clone_object; }

int main() {
    // fake objects: word 12 flags, floats 16..24, vtable word at 72
    alignas(8) static Byte objects[4][256], predicted[256];
    static VirtualEntry vtable[16];
    vtable[1] = {0, 0, handle_of(reinterpret_cast<void*>(h_destroy))};
    vtable[2] = {0, 0, handle_of(reinterpret_cast<void*>(h_damage))};
    vtable[15] = {0, 0, handle_of(reinterpret_cast<void*>(h_clone))};
    unsigned vt = handle_of(vtable);
    for (int i = 0; i < 4; ++i) {
        word(objects[i], 72) = vt;
        float pos[3] = {10.7f + i, -3.9f, 100.0f};
        std::memcpy(objects[i] + 16, pos, 12);
        word(player + i * 23984, 21872) = handle_of(objects[i]);
    }
    word(predicted, 72) = vt;
    word(predicted, 12) = 0x100;
    localPredictedPlayer = handle_of(predicted);
    word(objects[0], 12) = 0x400100;
    word(objects[1], 12) = 0x400000;
    for (int c = 0; c < 4; ++c) {
        word(compLocalCompressor, 16 * c) = 0; word(compLocalCompressor, 16 * c + 4) = 128;
        word(compLocalDecompressor, 16 * c) = 0; word(compLocalDecompressor, 16 * c + 4) = 128;
    }
    msgHandler[0] = handle_of(reinterpret_cast<void*>(h_msg));

    // two partners, partner 0 local
    netNetworkMode = 1;
    nmPartners = 2; nmPlayers = 2;
    trueNetFrameNm = 5; frameNm = 99;
    syncRandState = 1000; logChecksum = 23;
    netForceLocalUpdate = 3;
    std::memset(partners, 0, sizeof partners);
    partners[17620] = 1;
    for (int p = 0; p < 2; ++p) {
        Byte* record = partners + p * 17632;
        Byte* entry = record + 172 + 5 * 24;
        word(entry, 0) = 5;
        half(entry, 4) = 0x1234;
        entry[6] = 128; entry[7] = 128; entry[8] = 128; entry[9] = 128;
        word(entry, 12) = 0x00000444;  // even yaw, bit 0 clear
    }
    // a one-message queue for partner 1 at ring offset 8190 (wraps)
    {
        Byte* record = partners + 17632;
        Byte* ring = record + 12 + 9376;
        ring[8190] = 0; ring[8191] = 2; ring[0] = 77; ring[1] = 88;
        word(record + 172 + 5 * 24, 16) = 8190;
        word(record + 172 + 5 * 24, 20) = 4;
        word(record, 17622) = 0;
        record[17622] = 1;                // marked: kicked at this frame
        word(record, 17624) = 5;
        // partner 1's log for frame 6 matches, so it is compared
        word(record + 6316 + 6 * 12, 0) = 6;
    }
    advanceRealState__Fi(1);
    // order of effects: both pads, the message, the kick (damage), then core with
    // netTrueStep 1, route with 0, the compare for partner 1, then the rebuild.
    size_t pad1 = trace.find("pad pad");
    size_t dmg = trace.find("damage");
    size_t core = trace.find("core1");
    size_t route = trace.find("route0");
    size_t cmp = trace.find("cmp1");
    size_t clone = trace.find("clone");
    if (pad1 == std::string::npos || dmg == std::string::npos || core == std::string::npos ||
        route == std::string::npos || cmp == std::string::npos || clone == std::string::npos) {
        std::printf("trace: %s\n", trace.c_str());
        return 1;
    }
    if (!(pad1 < dmg && dmg < core && core < route && route < cmp && cmp < clone)) return 2;
    if (trace.find("msg77:2") == std::string::npos) return 3;       // ring wrapped past 8191
    if (trace.find("destroy3") == std::string::npos) return 4;
    if (trueNetFrameNm != 6) return 5;
    if (frameNm != 6 || derivedFrameNm != 6) return 6;
    Byte* log = partners + 6316 + 6 * 12;
    if (word(log, 0) != 6 || word(log, 4) != 99) return 7;
    // checksum: logChecksum + syncRandState + sum of truncated positions of both players
    int expect = 23 + 1000 + (10 - 3 + 100) + (11 - 3 + 100);
    if (word(log, 8) != expect) return 8;
    if (word(partners + 17632, 17584) != 2) return 9;            // ring position after the message
    if (word(partners, 17584) != 0) return 10;                   // the local partner keeps its own
    if (objectIgnoreFlags != 0x200000) return 11;
    if (netForceLocalUpdate != 2) return 12;
    if (netCameraYaw[0] != 0x444 || netCameraYaw[1] != 0x444) return 13;
    // the local frame went through the compressor and decompressor: buttons, analog
    // bytes and yaw are unchanged for a repeated value
    if (half(last_pad_data, 0) != 0x1234 || last_pad_data[2] != 128 || word(last_pad_data, 8) != 0x444) return 14;
    // kicked player lost bit 0x400000
    if ((word(objects[1], 12) & 0x400000) != 0) return 15;
    if (word(clone_object, 12) != ((0 & ~0x200000) | 0x20000 | 0x100)) return 16;
    if ((word(objects[0], 12) & 0x200000) == 0) return 17;
    if (netLogDisable != 0 || netPrediction != 0 || netTrueStep != 0) return 18;
    if (std::memcmp(compImmediateCompressor, compLocalCompressor, 76) != 0) return 19;
    if (compLocalCompressor[72] != 1 || compLocalDecompressor[72] != 1) return 20;
    return 0;
}
