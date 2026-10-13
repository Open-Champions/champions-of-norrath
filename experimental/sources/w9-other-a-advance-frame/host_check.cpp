#define NORRATH_HOST_CONTRACT 1
#include "advance_frame.cpp"
#include <cstdio>
#include <cstring>
#include <vector>

alignas(16) static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_w9_host_base = arena;
extern "C" {
int nmActors, displayFrame, eRandState, nmActions, displayActionPos, netWeSentReady, netNmReady, cutMsgHandler;
unsigned actor, action, dialogActor;
signed char cutSkipMode, cutPlayMode;
float cutTime;
}
static std::vector<int> trace;
static int playStatus, amountPlayed, partners, foundIndex;
extern "C" void norrath_w9_host_call_slot(unsigned function, unsigned self) { trace.push_back(1000 + function); trace.push_back(self); trace.push_back(eRandState); }
extern "C" void worldPlotRouteProcess__Fi(int v) { trace.push_back(2000 + v); }
extern "C" void executeAction__FP6Action(unsigned char* r) { trace.push_back(3000 + static_cast<int>(r - arena)); }
extern "C" int soundGetStreamPlayStatus__Fi(int) { return playStatus; }
extern "C" int soundGetStreamAmountPlayed__Fi(int) { return amountPlayed; }
extern "C" void netSendMessage__FiPUci(int h, unsigned char*, int) { trace.push_back(4000 + h); trace.push_back(netWeSentReady); }
extern "C" int netGetNmConnectedPartners__Fv() { return partners; }
extern "C" int animFindComponentWithFlags__FP14AnimationStatei(unsigned char*, int) { return foundIndex; }

static void w32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static void wf(unsigned at, float v) { std::memcpy(arena + at, &v, 4); }
static float rf(unsigned at) { float v; std::memcpy(&v, arena + at, 4); return v; }

int main() {
    // Part one: two actors, the second absent; slot entry has delta 16.
    actor = 0x100; nmActors = 2; displayFrame = 10;
    w32(0x100, 0x400);
    w32(0x400 + 72, 0x500);
    short delta = 16; std::memcpy(arena + 0x500 + 32, &delta, 2);
    w32(0x500 + 36, 77);
    action = 0x1000; nmActions = 0; foundIndex = -1;
    float r = advanceFrame__Ff(0.5f);
    if (r != 0.5f || displayFrame != 11 || eRandState != 11) return 1;
    int expect[] = {1077, 0x410, 10, 2001};
    if (trace != std::vector<int>(expect, expect + 4)) return 2;

    // Part two: three actions, the second is a handshake record that must wait.
    trace.clear(); nmActors = 0; nmActions = 3; displayActionPos = 0; displayFrame = 5;
    wf(0x1000, 0.0f); w32(0x1004, 1);
    wf(0x1014, 0.0f); w32(0x1018, 0x3ff9d);
    wf(0x1028, 0.0f); w32(0x102c, 1);
    cutTime = 1.0f; cutMsgHandler = 9; netNmReady = 0; partners = 0; playStatus = 0;
    cutPlayMode = 1; cutSkipMode = 0; netWeSentReady = 0;
    r = advanceFrame__Ff(1.0f);
    int expect2[] = {2001, 3000 + 0x1000, 4009, 1};
    if (trace != std::vector<int>(expect2, expect2 + 4)) return 9;
    if (displayFrame != 5 || displayActionPos != 1 || netWeSentReady != 1) return 3;
    if (r != 5.0f * constant(0x3c888888u) || cutTime != 1.0f - constant(0x3c888888u)) return 4;
    // Ready: the same record now runs executeAction and clears the handshake.
    trace.clear(); netNmReady = 1; displayActionPos = 1; displayFrame = 5;
    advanceFrame__Ff(1.0f);
    if (netWeSentReady != 0 || netNmReady != 0 || displayActionPos != 3) return 5;
    // Skip mode bypasses the handshake entirely.
    trace.clear(); cutSkipMode = 1; netNmReady = 0; displayActionPos = 1; netWeSentReady = 0;
    advanceFrame__Ff(1.0f);
    if (netWeSentReady != 0 || displayActionPos != 3) return 6;

    // Part three: correction sign and application.
    trace.clear(); nmActions = 0; dialogActor = 0x2000; foundIndex = 0; amountPlayed = 100000;
    arena[0x2000 + 288 + 4] = 2;
    wf(0x2000 + 320, 0.0f);
    wf(0x2000 + 320, 0.0f); wf(0x2000 + 288 + 32, 1.0f); wf(0x2000 + 288 + 64, 2.0f);
    advanceFrame__Ff(0.0f);
    if (rf(0x2000 + 288 + 32) != 1.0f + constant(0x3dccccccu)) return 7;
    if (rf(0x2000 + 288 + 64) != 2.0f + constant(0x3dccccccu)) return 8;
    return 0;
}
