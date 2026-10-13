// Host check for updateTaskBar__FiP12screenStruct. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
#include "update_task_bar.cpp"

#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>

extern "C" {
signed char multiHelpMessageOn[4];
int sellMode[4];
signed char helpMessageOn;
int shopRunning;
unsigned padAcceptButton = 0x4000;
alignas(8) unsigned char netLocalPad[4 * 88];
alignas(8) unsigned char pad[4 * 88];
alignas(8) unsigned char statsScreen[4 * 116];
}
static std::string trace;
static int hotAnswer, localAnswer;
static char seq;
extern "C" int shopMouseOnHotSpot__FP12screenStruct(void* screen) { trace += screen ? "hot;" : "hot?;"; return hotAnswer; }
extern "C" int netPlayerIsLocal__Fi(int player) { trace += "local" + std::to_string(player) + ";"; return localAnswer; }
extern "C" void* lumpFindResource__FPcT0(char* lump, char* resource) { trace += std::string("find ") + lump + " " + resource + ";"; return &seq; }
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume) {
    trace += "snd" + std::string(sequence == &seq ? "" : "?") + (object == nullptr ? "" : "?") + std::to_string(static_cast<int>(volume * 10)) + ";";
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)

static unsigned& at(unsigned char* base, int off) { return *reinterpret_cast<unsigned*>(base + off); }

struct Screen { int w[32]; };
static void reset(Screen& s, int tab) {
    trace.clear();
    for (int i = 0; i < 32; ++i) s.w[i] = 100 + i;
    s.w[20] = tab;
    std::memset(multiHelpMessageOn, 0, sizeof multiHelpMessageOn);
    std::memset(sellMode, 0, sizeof sellMode);
    helpMessageOn = 0; shopRunning = 0; padAcceptButton = 0x4000;
    for (int i = 0; i < 4 * 88; ++i) { netLocalPad[i] = static_cast<unsigned char>(i * 3); pad[i] = static_cast<unsigned char>(i * 5 + 1); }
    for (int i = 0; i < 4 * 116; ++i) statsScreen[i] = static_cast<unsigned char>(i + 7);
    for (int q = 0; q < 4; ++q) { at(netLocalPad, 88 * q + 12) = 0; at(netLocalPad, 88 * q + 16) = 0; }
    hotAnswer = -1; localAnswer = 1;
}
static bool changedOnly(const Screen& before, const Screen& after, std::initializer_list<int> words) {
    for (int i = 0; i < 32; ++i) {
        bool listed = false;
        for (int w : words) if (w == i) listed = true;
        if (!listed && before.w[i] != after.w[i]) return false;
    }
    return true;
}

int main() {
    Screen s, b;
    // 1. a click on a hot spot: tab = hot spot, words cleared, held bits of both pads cleared, tab sound
    reset(s, 2); at(netLocalPad, 88 * 1 + 12) = 0x4000 | 1; at(netLocalPad, 88 * 1 + 16) = 0xff; hotAnswer = 4;
    b = s;
    unsigned char padBefore[4 * 88], statsBefore[4 * 116];
    std::memcpy(padBefore, pad, sizeof pad); std::memcpy(statsBefore, statsScreen, sizeof statsScreen);
    updateTaskBar__FiP12screenStruct(1, &s);
    CHECK(trace == "hot;local1;find fx.lmp game_butn_tab.anm;snd10;");
    CHECK(s.w[20] == 4 && s.w[5] == 0 && s.w[6] == 0 && s.w[7] == 0 && s.w[8] == 0 && s.w[9] == 0 && s.w[10] == 0);
    CHECK(changedOnly(b, s, {5, 6, 7, 8, 9, 10, 20}) && s.w[12] == b.w[12]);
    CHECK(at(netLocalPad, 88 + 16) == 0 && at(pad, 88 + 16) == 0);
    CHECK(at(netLocalPad, 88 + 12) == (0x4000u | 1u));
    for (int i = 0; i < 4 * 88; ++i) {
        if (i >= 88 + 16 && i < 88 + 20) continue;
        CHECK(pad[i] == padBefore[i]);
    }
    for (int i = 0; i < 4 * 116; ++i) {
        const int rel = i - 116 * 1;
        const bool cleared = (rel >= 24 && rel < 28) || (rel >= 36 && rel < 44);   // words 6, 9 and 10 of the record
        CHECK(statsScreen[i] == (cleared ? 0 : statsBefore[i]));
    }
    // non-local player: no sound
    reset(s, 2); at(netLocalPad, 12) = 0x4000; hotAnswer = 0; localAnswer = 0;
    updateTaskBar__FiP12screenStruct(0, &s);
    CHECK(trace == "hot;local0;" && s.w[20] == 0);
    // the hot spot must be non-negative; a click without a hot spot falls through to the shoulder buttons
    reset(s, 2); at(netLocalPad, 12) = 0x4000; at(netLocalPad, 16) = 8; hotAnswer = -1;
    updateTaskBar__FiP12screenStruct(0, &s);
    CHECK(s.w[20] == 3 && trace == "hot;local0;find fx.lmp menu_shop_tab.anm;snd10;");
    // the accept button must be held for the click: other pressed bits do nothing
    reset(s, 2); at(netLocalPad, 12) = 0x3fff; hotAnswer = 4;
    updateTaskBar__FiP12screenStruct(0, &s);
    CHECK(trace == "hot;" && s.w[20] == 2);
    // 2. shoulder buttons: raise with 8 or 2, lower with 4 or 1, within 0..last-2
    for (unsigned up : {8u, 2u, 10u}) {
        reset(s, 1); at(netLocalPad, 88 * 2 + 16) = up; b = s;
        updateTaskBar__FiP12screenStruct(2, &s);
        CHECK(trace == "hot;local2;find fx.lmp menu_shop_tab.anm;snd10;");
        CHECK(s.w[20] == 2 && s.w[12] == 0 && s.w[5] == 0 && s.w[6] == 0 && s.w[7] == 0 && s.w[8] == 0 && s.w[9] == 0 && s.w[10] == 0);
        CHECK(changedOnly(b, s, {5, 6, 7, 8, 9, 10, 12, 20}));
        CHECK(at(netLocalPad, 88 * 2 + 16) == up);   // the held bits are left alone
    }
    for (unsigned down : {4u, 1u, 5u}) {
        reset(s, 3); at(netLocalPad, 16) = down;
        updateTaskBar__FiP12screenStruct(0, &s);
        CHECK(s.w[20] == 2 && trace == "hot;local0;find fx.lmp menu_shop_tab.anm;snd10;");
    }
    // limits: raise stops above last - 2 (4 for the six-tab bar, 1 when the shop is running), lower stops at 0
    reset(s, 4); at(netLocalPad, 16) = 8; updateTaskBar__FiP12screenStruct(0, &s);
    CHECK(s.w[20] == 5 && s.w[5] == 0);
    reset(s, 5); at(netLocalPad, 16) = 8; b = s; updateTaskBar__FiP12screenStruct(0, &s);
    CHECK(s.w[20] == 5 && s.w[5] == 0 && s.w[12] == b.w[12] && trace == "hot;");
    reset(s, 1); shopRunning = 7; at(netLocalPad, 16) = 8; updateTaskBar__FiP12screenStruct(0, &s);
    CHECK(s.w[20] == 2);
    reset(s, 2); shopRunning = 7; at(netLocalPad, 16) = 8; b = s; updateTaskBar__FiP12screenStruct(0, &s);
    CHECK(s.w[20] == 2 && trace == "hot;" && s.w[12] == b.w[12]);
    reset(s, 0); at(netLocalPad, 16) = 4; b = s; updateTaskBar__FiP12screenStruct(0, &s);
    CHECK(s.w[20] == 0 && s.w[5] == 0 && trace == "hot;" && s.w[12] == b.w[12]);
    reset(s, -3); at(netLocalPad, 16) = 4; updateTaskBar__FiP12screenStruct(0, &s);
    CHECK(s.w[20] == -3 && trace == "hot;");
    // no shoulder bits: the latch word is not cleared either
    reset(s, 2); at(netLocalPad, 16) = 0xf0; b = s; updateTaskBar__FiP12screenStruct(0, &s);
    CHECK(trace == "hot;" && std::memcmp(&b, &s, sizeof s) == 0);
    // 3. the early returns: help message, sell mode, multiplayer help for this player only
    for (int which = 0; which < 3; ++which) {
        reset(s, 2); at(netLocalPad, 88 * 3 + 16) = 8; at(netLocalPad, 88 * 3 + 12) = 0x4000; hotAnswer = 1;
        if (which == 0) multiHelpMessageOn[3] = 1;
        if (which == 1) sellMode[3] = 1;
        if (which == 2) helpMessageOn = static_cast<signed char>(0xff);
        updateTaskBar__FiP12screenStruct(3, &s);
        CHECK(trace.empty() && s.w[20] == 2);
    }
    reset(s, 2); at(netLocalPad, 88 * 3 + 16) = 8; multiHelpMessageOn[2] = 1; sellMode[2] = 1;
    updateTaskBar__FiP12screenStruct(3, &s);
    CHECK(s.w[20] == 3);   // other players' flags do not matter
    std::puts("update task bar passed");
    return 0;
}
