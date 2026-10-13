// Host check for playSoundGroup__FP15AnimationHeaderiiP6Point3. Not an EE plan
// input. Record words hold offsets into an arena (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include <cstring>
#include "play_sound_group.cpp"
extern "C" {
int eRandState;
void (*specialHandler)(int code, Point3 position);
alignas(16) static unsigned char arena[0x4000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
struct Played { unsigned sound; Point3* position; unsigned char* event; int byte; };
static Played played[16];
static int nPlayed;
void animPlaySound__FPvP6Point3P9animSoundi(void* sound, Point3* position, void* event, int groupByte) {
    played[nPlayed++] = {static_cast<unsigned>(static_cast<unsigned char*>(sound) - arena), position,
                         static_cast<unsigned char*>(event), groupByte};
}
static int handled[4], nHandled;
static Point3 handledAt;
static void handler(int code, Point3 position) { handled[nHandled++] = code; handledAt = position; }
}
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void put16(unsigned off, short v) { std::memcpy(arena + off, &v, 2); }
int main() {
    const unsigned header = 0x100, events = 0x400, sounds = 0x800;
    put32(header + 52, sounds);
    put32(header + 56, events);
    arena[header + 60] = 77;
    for (int j = 0; j < 4; ++j) put32(sounds + 8 * j + 4, 0x1000 + 0x10 * j);
    Point3 pos = {1.5f, 2.5f, 3.5f};
    auto event = [&](int i, int type, int code) {
        arena[events + 28 * i + 8] = static_cast<unsigned char>(type);
        put16(events + 28 * i + 4, static_cast<short>(code));
    };
    // Two events of one group: exactly one plays whatever the generator rolls.
    for (int seed = 0; seed < 2000; ++seed) {
        eRandState = seed * 40503 + 7;
        unsigned s = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
        float roll = static_cast<float>(s >> 16) * (1.0f / 65536.0f);
        event(0, 3, 1);
        event(1, 3, 2);
        nPlayed = 0;
        playSoundGroup__FP15AnimationHeaderiiP6Point3(arena + header, 0, 2, &pos);
        if (nPlayed != 1) return 1;
        bool first = roll <= 0.5f;
        if (played[0].sound != (first ? 0x1010u : 0x1020u)) return 2;
        if (played[0].event != arena + events + (first ? 0 : 28)) return 3;
        // Rolling for the second event too costs one more generator step.
        unsigned s2 = s * 0x19660du + 0x3c6ef35fu;
        if (eRandState != static_cast<int>(first ? s : s2)) return 4;
        if (played[0].byte != 77 || played[0].position != &pos) return 6;
    }
    // Different groups both play; type 0 and type 7 events play without a roll.
    event(0, 1, 0); event(1, 2, 3); event(2, 0, 2); event(3, 7, 1);
    eRandState = 5;
    nPlayed = 0;
    playSoundGroup__FP15AnimationHeaderiiP6Point3(arena + header, 0, 4, &pos);
    if (nPlayed != 4) return 7;
    // A range that stops early leaves later events alone.
    nPlayed = 0;
    playSoundGroup__FP15AnimationHeaderiiP6Point3(arena + header, 2, 3, &pos);
    if (nPlayed != 1 || played[0].event != arena + events + 56) return 8;
    nPlayed = 0;
    playSoundGroup__FP15AnimationHeaderiiP6Point3(arena + header, 3, 3, &pos);
    if (nPlayed != 0) return 9;
    // Negative codes: -1 does nothing; below -1 calls the handler with a copy.
    event(0, 0, -1); event(1, 0, -2); event(2, 0, -9);
    specialHandler = 0;
    nPlayed = 0;
    playSoundGroup__FP15AnimationHeaderiiP6Point3(arena + header, 0, 3, &pos);
    if (nPlayed != 0 || nHandled != 0) return 10;
    specialHandler = handler;
    playSoundGroup__FP15AnimationHeaderiiP6Point3(arena + header, 0, 3, &pos);
    if (nPlayed != 0 || nHandled != 2 || handled[0] != -2 || handled[1] != -9) return 11;
    if (handledAt.x != 1.5f || handledAt.y != 2.5f || handledAt.z != 3.5f) return 12;
    return 0;
}
