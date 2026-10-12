#include <cstdio>
#include <cstring>
extern "C" {
unsigned char delaySoundList[32 * 36] = {};
unsigned char batchBuffer[512 * 8] = {};
unsigned char norrath_sound_channel_records_view[860] = {};
unsigned char norrath_sound_channel_voice_map_view[86] = {};
unsigned char norrath_sound_sequence_records_view[2432] = {};
unsigned char norrath_sound_key_on_words_view[8] = {};
unsigned char norrath_sound_key_off_words_view[8] = {};
int disableSoundPlaying = 0;
int nmBatchCommands = 0;
int sequenceInProgress = -1;
int soundFrameNm = 1000;
float sequenceVolume = 0.5f;
}
struct Point3 { float x, y, z; };
static int channel_result = 4;
static int volume_calls = 0, pan_calls = 0, find_calls = 0;
static int volume_channel;
static float volume_left, volume_right;
static Point3 pan_position;
static float pan_a, pan_b;
static float pan_left = 0.25f, pan_right = 0.75f;
extern "C" int findChannel__Fv() { find_calls += 1; return channel_result; }
extern "C" void pan__FG6Point3ffPfT3(Point3 position, float a, float b, float* left, float* right) {
    pan_calls += 1; pan_position = position; pan_a = a; pan_b = b; *left = pan_left; *right = pan_right;
}
extern "C" void soundChangeVolume__Fiff(int channel, float left, float right) {
    volume_calls += 1; volume_channel = channel; volume_left = left; volume_right = right;
}
extern "C" int soundPlaySound__FP10_vagHeaderP6Point3fff(unsigned char*, unsigned char*, float, float, float);
static unsigned word(const unsigned char* at) { unsigned v; std::memcpy(&v, at, 4); return v; }
static unsigned half(const unsigned char* at) { return at[0] | (at[1] << 8); }
static void set_word(unsigned char* at, unsigned v) { std::memcpy(at, &v, 4); }
static float fl(const unsigned char* at) { float v; std::memcpy(&v, at, 4); return v; }
static unsigned address(const void* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); }
int main() {
    alignas(8) unsigned char vag[64] = {};
    set_word(vag + 32, 10000);
    set_word(vag + 36, 0x1234);
    float position[3] = {1, 2, 3};
    unsigned char* pos = reinterpret_cast<unsigned char*>(position);
    disableSoundPlaying = 1;
    if (soundPlaySound__FP10_vagHeaderP6Point3fff(vag, pos, 1, 1, 0) != -1 || find_calls) return 1;
    disableSoundPlaying = 0;

    // delayed
    sequenceInProgress = 5;
    if (soundPlaySound__FP10_vagHeaderP6Point3fff(vag, pos, 0.5f, 2.0f, 1.5f) != -1 || find_calls) return 2;
    unsigned char* e = delaySoundList;
    if (word(e) != address(vag) || fl(e + 4) != 1 || fl(e + 12) != 3 || e[32] != 1) return 3;
    if (fl(e + 16) != 0.5f || fl(e + 20) != 1.0f || word(e + 24) != 1090 || word(e + 28) != 5) return 4;
    soundPlaySound__FP10_vagHeaderP6Point3fff(vag, nullptr, 1, 1, 0.25f);
    e = delaySoundList + 36;
    if (word(e + 24) != 1015 || e[32] != 0) return 5;
    for (int i = 2; i < 32; ++i) set_word(delaySoundList + i * 36 + 24, 1);
    if (soundPlaySound__FP10_vagHeaderP6Point3fff(vag, pos, 1, 1, 1) != -1) return 6;
    if (word(delaySoundList + 36 * 31 + 24) != 1) return 7;

    // immediate, no sequence
    sequenceInProgress = -1;
    norrath_sound_channel_voice_map_view[8] = 1;
    norrath_sound_channel_voice_map_view[9] = 3;
    int got = soundPlaySound__FP10_vagHeaderP6Point3fff(vag, pos, 2.0f, 4.0f, 0);
    if (got != 4 || find_calls != 1) return 8;
    unsigned char* c = norrath_sound_channel_records_view + 80;
    if (word(c) != address(vag) || half(c + 4) != 4 || word(c + 12) != 16383) return 9;
    if (half(c + 6) != 0xffff || fl(c + 16) != 4.0f) return 10;
    if (pan_calls != 1 || pan_a != 360.0f || pan_b != 840.0f || pan_position.z != 3) return 11;
    if (volume_channel != 4 || volume_left != 0.25f * 2.0f || volume_right != 0.75f * 2.0f) return 12;
    if (nmBatchCommands != 5) return 13;
    unsigned flags = 1 | (3 << 1);
    if (half(batchBuffer) != 1 || half(batchBuffer + 2) != (flags | 0x200) || word(batchBuffer + 4) != 16383) return 14;
    if (half(batchBuffer + 8) != 3 || half(batchBuffer + 10) != (flags | 0x2040) || word(batchBuffer + 12) != 0x1234) return 15;
    if (half(batchBuffer + 18) != (flags | 0x300) || word(batchBuffer + 20) != 255) return 16;
    if (half(batchBuffer + 26) != (flags | 0x400) || word(batchBuffer + 28) != 8128) return 17;
    if (half(batchBuffer + 32) != 3 || half(batchBuffer + 34) != (flags | 0x2140) || word(batchBuffer + 36) != 20480) return 18;
    if (word(norrath_sound_key_on_words_view + 4) != 8 || word(norrath_sound_key_off_words_view + 4) != 0) return 19;
    set_word(norrath_sound_key_off_words_view + 4, 0xffffffffu);
    // negative level clamps to zero; no position gives unit pan
    set_word(vag + 32, static_cast<unsigned>(-5000));
    soundPlaySound__FP10_vagHeaderP6Point3fff(vag, nullptr, 1.0f, 1.0f, 0);
    if (word(c + 12) != 0 || volume_left != 0.5f || volume_right != 0.5f) return 20;
    if (word(norrath_sound_key_off_words_view + 4) != 0xfffffff7u) return 21;
    // silence rejects, unless the flagged halfword is set
    pan_left = 0; pan_right = 0; volume_calls = 0;
    if (soundPlaySound__FP10_vagHeaderP6Point3fff(vag, pos, 1, 1, 0) != -1 || volume_calls != 0) return 22;
    // flagged sound requires a sequence and rejects duplicates
    vag[40] = 1;
    sequenceInProgress = -1;
    find_calls = 0;
    if (soundPlaySound__FP10_vagHeaderP6Point3fff(vag, pos, 1, 1, 0) != -1 || find_calls) return 23;
    sequenceInProgress = 7;
    set_word(norrath_sound_channel_records_view + 20 * 10, address(vag));
    norrath_sound_channel_records_view[20 * 10 + 6] = 7;
    if (soundPlaySound__FP10_vagHeaderP6Point3fff(vag, pos, 1, 1, 0) != -1 || find_calls) return 24;
    norrath_sound_channel_records_view[20 * 10 + 6] = 8;
    unsigned char* seq = norrath_sound_sequence_records_view + 7 * 76;
    seq[56] = 0x21; seq[59] = 1;
    set_word(seq + 68, 0x40a00000); set_word(seq + 72, 0x40c00000);
    got = soundPlaySound__FP10_vagHeaderP6Point3fff(vag, pos, 1, 1, 0);
    if (got != 4 || half(c + 4) != (20 | 8) || half(c + 6) != 7 || half(c + 8) != 0x21) return 25;
    if (pan_a != 5.0f || pan_b != 6.0f || volume_calls != 1) return 26;
    std::puts("play sound contract passed");
    return 0;
}
