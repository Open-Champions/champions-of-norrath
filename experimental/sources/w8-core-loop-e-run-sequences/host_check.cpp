// Host check for runSequences__Fv. Not an EE plan input.
#define NORRATH_HOST_CONTRACT
#include "run_sequences.cpp"

#include <cstring>
#include <string>

extern "C" {
unsigned char norrath_sound_sequence_records_view[32 * 76];
unsigned char norrath_sound_channel_records_view[860];
unsigned char worldCameraPos[12];
int sequenceInProgress = 5;
float sequenceVolume = 0.25f;
}
static unsigned char arena[1 << 16];
extern "C" void* norrath_host_pointer(unsigned address) { return arena + address; }

static std::string trace;
static int paused = 0;
extern "C" int gameIsPaused__Fv() { trace += "P"; return paused; }
extern "C" void soundStopSound__Fi(int channel) { trace += "X" + std::to_string(channel) + ";"; }
static unsigned char position_seen[12];
extern "C" void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* state, void* position, void* second, short* third) {
    const long index = ((unsigned char*)state - norrath_sound_sequence_records_view) / 76;
    trace += "A" + std::to_string(index) + "/" + std::to_string(sequenceInProgress) + "/" + std::to_string(sequenceVolume) + "/"
        + ((unsigned char*)position == (unsigned char*)state + 44 ? "p" : "?") + (second || third ? "?" : "") + ";";
    std::memcpy(position_seen, position, 12);
}

static void w32(unsigned char* p, unsigned v) { for (int i = 0; i < 4; ++i) p[i] = (unsigned char)(v >> (8 * i)); }
static void w16(unsigned char* p, unsigned v) { p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8); }
static void wf(unsigned char* p, float f) { std::memcpy(p, &f, 4); }
static unsigned char* rec(int i) { return norrath_sound_sequence_records_view + i * 76; }
static unsigned char* chan(int i) { return norrath_sound_channel_records_view + i * 20; }

int main() {
    // Header at arena 0x100 with table at 0x200 (28-byte entries) and countdown int 100 at +64.
    w32(arena + 0x100 + 56, 0x200);
    w32(arena + 0x100 + 64, 100);
    // Entry 2 is a live entry; entry 4 holds the end sentinel.
    w32(arena + 0x200 + 2 * 28, 1234);
    w32(arena + 0x200 + 4 * 28, 0x7fffffffu);
    // List head at 0x300 whose +12 word points to object 0x400; object+16 holds a position.
    w32(arena + 0x300 + 12, 0x400);
    for (int i = 0; i < 12; ++i) arena[0x400 + 16 + i] = (unsigned char)(0x10 + i);
    for (int i = 0; i < 12; ++i) worldCameraPos[i] = (unsigned char)(0x80 + i);

    // No live record: only the pause read and the final stores.
    runSequences__Fv();
    if (trace != "P" || sequenceInProgress != -1 || sequenceVolume != 1.0f) return 1;

    // Record 3: running (group byte 0, entry 2), position from the object list, volume 0.75.
    trace.clear();
    rec(3)[4] = 1; w32(rec(3) + 8, 0x100); w16(rec(3) + 38, 2); w32(rec(3) + 40, 0x300); wf(rec(3) + 64, 0.75f);
    runSequences__Fv();
    if (trace != "PA3/3/0.750000/p;") return 2;
    for (int i = 0; i < 12; ++i) if (position_seen[i] != 0x10 + i) return 3;
    if (rec(3)[4] != 1) return 4;

    // Listener override: byte +58 replaces the position with worldCameraPos.
    trace.clear(); rec(3)[58] = 1;
    runSequences__Fv();
    for (int i = 0; i < 12; ++i) if (position_seen[i] != 0x80 + i) return 5;

    // Paused with byte +59 clear: skipped; with +59 set: runs.
    trace.clear(); paused = 1;
    runSequences__Fv();
    if (trace != "P") return 6;
    rec(3)[59] = 1;
    trace.clear();
    runSequences__Fv();
    if (trace != "PA3/3/0.750000/p;") return 7;
    paused = 0;

    // Sentinel entry ends the record and stops matching channels (only).
    trace.clear(); rec(3)[59] = 0; w16(rec(3) + 38, 4); w16(rec(3) + 56, 9);
    w16(chan(5) + 4, 0x14); w16(chan(5) + 6, 3); w16(chan(5) + 8, 9);        // matches
    w16(chan(6) + 4, 0x04); w16(chan(6) + 6, 3); w16(chan(6) + 8, 9);        // missing 0x10
    w16(chan(7) + 4, 0x10); w16(chan(7) + 6, 3); w16(chan(7) + 8, 9);        // missing 4
    w16(chan(8) + 4, 0x14); w16(chan(8) + 6, 2); w16(chan(8) + 8, 9);        // other record
    w16(chan(9) + 4, 0x14); w16(chan(9) + 6, 3); w16(chan(9) + 8, 8);        // other group
    w16(chan(42) + 4, 0x14); w16(chan(42) + 6, 3); w16(chan(42) + 8, 9);     // last record is searched
    runSequences__Fv();
    if (trace != "PX5;X42;") return 8;
    if (rec(3)[4] != 0) return 9;

    // Flag 0x80 ends a record whose entry is otherwise fine.
    trace.clear(); rec(3)[4] = 1; w16(rec(3) + 38, 2); w16(rec(3) + 6, 0x80);
    runSequences__Fv();
    if (trace != "PX5;X42;" || rec(3)[4] != 0) return 10;

    // Missing table ends the record.
    trace.clear(); rec(3)[4] = 1; w16(rec(3) + 6, 0); w32(arena + 0x100 + 56, 0);
    runSequences__Fv();
    if (rec(3)[4] != 0) return 11;
    w32(arena + 0x100 + 56, 0x200);

    // Group byte nonzero: no end check; the countdown float is zeroed when above the header count.
    trace.clear(); rec(3)[4] = 1; rec(3)[60] = 1; w16(rec(3) + 6, 0x80); wf(rec(3) + 32, 150.0f);
    runSequences__Fv();
    float f; std::memcpy(&f, rec(3) + 32, 4);
    if (f != 0.0f || trace != "PA3/3/0.750000/p;") return 12;
    wf(rec(3) + 32, 50.0f); trace.clear();
    runSequences__Fv();
    std::memcpy(&f, rec(3) + 32, 4);
    if (f != 50.0f) return 13;
    // Equal values are left alone.
    wf(rec(3) + 32, 100.0f);
    runSequences__Fv();
    std::memcpy(&f, rec(3) + 32, 4);
    if (f != 100.0f) return 14;

    // Two live records run in index order.
    trace.clear(); rec(1)[4] = 1; w32(rec(1) + 8, 0x100); w16(rec(1) + 38, 2); rec(1)[60] = 0; w16(rec(1) + 6, 0);
    w32(rec(1) + 40, 0); wf(rec(1) + 64, 2.0f);
    runSequences__Fv();
    if (trace != "PA1/1/2.000000/p;A3/3/0.750000/p;") return 15;
    return 0;
}
