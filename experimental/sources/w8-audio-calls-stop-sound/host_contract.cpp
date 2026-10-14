#include "stop_sound.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned char sequence[2432] = {};
extern "C" unsigned char norrath_sound_channel_records_view[43 * 20 + 4] = {};
extern "C" unsigned char norrath_sound_channel_voice_map_view[86] = {};
extern "C" unsigned char norrath_sound_key_on_words_view[8] = {};
extern "C" unsigned char norrath_sound_key_off_words_view[8] = {};
static void half(unsigned char* at, unsigned v) { std::memcpy(at, &v, 2); }
static unsigned short gh(const unsigned char* at) { unsigned short v; std::memcpy(&v, at, 2); return v; }
static unsigned char* ch(int c) { return norrath_sound_channel_records_view + c * 20; }
int main() {
    for (int c = 0; c < 43; ++c) { norrath_sound_channel_voice_map_view[c * 2] = c % 2; norrath_sound_channel_voice_map_view[c * 2 + 1] = c % 32; }
    unsigned* on = reinterpret_cast<unsigned*>(norrath_sound_key_on_words_view);
    unsigned* off = reinterpret_cast<unsigned*>(norrath_sound_key_off_words_view);
    // -1 changes nothing
    on[0] = on[1] = 0xffffffffu; half(ch(5) + 4, 7);
    soundStopSound__Fi(-1);
    if (on[0] != 0xffffffffu || gh(ch(5) + 4) != 7) return 1;
    // channel 5: word 1, bit 5
    soundStopSound__Fi(5);
    if (gh(ch(5) + 4) != 0 || off[1] != (1u << 5) || off[0] != 0) return 2;
    if (on[1] != ~(1u << 5) || on[0] != 0xffffffffu) return 3;
    half(ch(5) + 6, 0x55);     // other bytes of the record are untouched
    soundStopSound__Fi(5);
    if (gh(ch(5) + 6) != 0x55) return 4;
    // sequence id: index 3, spinner 0x12
    unsigned char* rec = sequence + 3 * 76;
    rec[4] = 1; half(rec + 56, 0x12);
    half(ch(10) + 4, 4); half(ch(10) + 6, 3); half(ch(10) + 8, 0x12);            // match
    half(ch(11) + 4, 3); half(ch(11) + 6, 3); half(ch(11) + 8, 0x12);            // bit 2 missing
    half(ch(12) + 4, 4); half(ch(12) + 6, 4); half(ch(12) + 8, 0x12);            // other sequence
    half(ch(13) + 4, 5); half(ch(13) + 6, 3); half(ch(13) + 8, 0x13);            // other spinner
    half(ch(14) + 4, 0x84); half(ch(14) + 6, 3); half(ch(14) + 8, 0x12);         // extra bits are fine
    soundStopSound__Fi(3 | (0x12 << 8) | 0x8000);
    if (rec[4] != 0) return 5;
    if (gh(ch(10) + 4) != 0 || gh(ch(14) + 4) != 0) return 6;
    if (gh(ch(11) + 4) != 3 || gh(ch(12) + 4) != 4 || gh(ch(13) + 4) != 5) return 7;
    // spinner mismatch: the record stays, matching channels are still stopped
    rec[4] = 1; half(rec + 56, 0x20); half(ch(10) + 4, 4); half(ch(10) + 8, 0x12);
    soundStopSound__Fi(3 | (0x12 << 8) | 0x8000);
    if (rec[4] != 1 || gh(ch(10) + 4) != 0) return 8;
    // the spinner uses only 7 bits of the id
    rec[4] = 1; half(rec + 56, 0x7f);
    soundStopSound__Fi(3 | (0xff << 8) | 0x8000);
    if (rec[4] != 0) return 9;
    std::puts("stop_sound passed");
    return 0;
}
