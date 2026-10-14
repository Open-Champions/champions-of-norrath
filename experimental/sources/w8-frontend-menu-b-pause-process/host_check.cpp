#include "pause_process.cpp"

#include <cstdio>
#include <cstring>

extern "C" {
unsigned char norrath_sound_channel_records_view[43 * 20];
unsigned char norrath_sound_channel_voice_map_view[86];
unsigned char batchBuffer[512 * 8];
int nmBatchCommands = 0;
signed char paused = 0;
}

static int is_paused;
static int pause_calls;
extern "C" int gameIsPaused__Fv() { ++pause_calls; return is_paused; }

static int failures;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)

static unsigned short half(const unsigned char* at) { return at[0] | (at[1] << 8); }
static unsigned word(const unsigned char* at) { return at[0] | (at[1] << 8) | (at[2] << 16) | (at[3] << 24); }
static void set_half(unsigned char* at, unsigned v) { at[0] = v; at[1] = v >> 8; }
static void set_word(unsigned char* at, unsigned v) { set_half(at, v); set_half(at + 2, v >> 16); }
static unsigned char* rec(int i) { return norrath_sound_channel_records_view + i * 20; }

int main() {
    for (int i = 0; i < 43; ++i) {
        norrath_sound_channel_voice_map_view[i * 2] = i & 1;
        norrath_sound_channel_voice_map_view[i * 2 + 1] = i;
    }
    // Channel 0: flags 1 (held). 1: flags 4 (held). 2: flags 5|8 (skipped).
    // 3: flags 0 (idle). 42: flags 1 with voice byte 42.
    set_half(rec(0) + 4, 1);
    set_half(rec(1) + 4, 4);
    set_half(rec(2) + 4, 13);
    set_half(rec(42) + 4, 1);
    set_word(rec(0) + 12, 1111);
    set_word(rec(1) + 12, 2222);
    set_word(rec(42) + 12, 4242);

    // Not paused and paused byte clear: nothing happens.
    is_paused = 0;
    pauseProcess__Fv();
    CHECK(nmBatchCommands == 0);
    CHECK(paused == 0);

    // Pause begins.
    is_paused = 1;
    pauseProcess__Fv();
    CHECK(paused == 1);
    CHECK(nmBatchCommands == 3);
    CHECK(half(batchBuffer) == 1);
    CHECK(half(batchBuffer + 2) == (0u | (0u << 1) | 0x200u));    // channel 0
    CHECK(word(batchBuffer + 4) == 0);
    CHECK(half(batchBuffer + 8 + 2) == (1u | (1u << 1) | 0x200u)); // channel 1
    CHECK(half(batchBuffer + 16 + 2) == (0u | (42u << 1) | 0x200u)); // channel 42
    CHECK(half(rec(0) + 4) == 3);
    CHECK(half(rec(1) + 4) == 6);
    CHECK(half(rec(2) + 4) == 13);
    CHECK(half(rec(3) + 4) == 0);

    // Still paused: a second call changes nothing.
    pauseProcess__Fv();
    CHECK(nmBatchCommands == 3);

    // Resume: held channels get their saved volume word, flag bit 2 clears.
    is_paused = 0;
    pauseProcess__Fv();
    CHECK(paused == 0);
    CHECK(nmBatchCommands == 6);
    CHECK(half(batchBuffer + 24) == 1);
    CHECK(word(batchBuffer + 24 + 4) == 1111);
    CHECK(word(batchBuffer + 32 + 4) == 2222);
    CHECK(word(batchBuffer + 40 + 4) == 4242);
    CHECK(half(rec(0) + 4) == 1);
    CHECK(half(rec(1) + 4) == 4);
    CHECK(half(rec(2) + 4) == 13);

    // The batch limit of 512 stops appends but flags are still updated.
    nmBatchCommands = 511;
    paused = 0;
    is_paused = 1;
    pauseProcess__Fv();
    CHECK(nmBatchCommands == 512);
    CHECK(half(rec(0) + 4) == 3);
    CHECK(half(rec(1) + 4) == 6);
    CHECK(half(rec(42) + 4) == 3);

    if (failures == 0) std::printf("pause_process ok\n");
    return failures != 0;
}
