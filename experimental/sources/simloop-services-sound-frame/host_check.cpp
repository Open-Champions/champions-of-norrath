#include <cstdio>
#include <cstring>
extern "C" {
unsigned char delaySoundList[32 * 36] = {};
unsigned char norrath_sound_channel_records_view[860] = {};
unsigned char norrath_sound_channel_voice_map_view[86] = {};
int soundFrameNm = 8;
int sequenceInProgress = 77;
int disableSoundPlaying = 3;
float soundMusicVolume = 0.25f;
}
static char log_text[256];
static int log_length = 0;
static void note(char tag) { log_text[log_length++] = tag; log_text[log_length] = 0; }
struct Play { unsigned header; bool has_position; float gain, scale, delay; int sequence; };
static Play plays[8];
static int play_count = 0;
static int status_result = 0;
static int stream_volume_calls = 0;
static float stream_volume;
static int adjust_calls = 0;
static int remote_calls = 0;
static int remote_words[2] = {0, 0};
extern "C" {
void runMusicDucking__Fv() { note('d'); }
void pauseProcess__Fv() { note('p'); }
void runScreaming__Fv() { note('s'); }
void runSequences__Fv() { note('q'); }
void runAmbient__Fv() { note('a'); }
void soundFlushCommandBuffer__Fv() { note('f'); }
void soundUpdateAdjustments__Fv() { adjust_calls += 1; }
int soundGetStreamPlayStatus__Fi(int index) { return index == 0 ? status_result : -1; }
void soundSetStreamVolume__Fif(int index, float volume) { if (index == 0) { stream_volume_calls += 1; stream_volume = volume; } }
int soundPlaySound__FP10_vagHeaderP6Point3fff(void* header, void* position, float gain, float scale, float delay) {
    Play& p = plays[play_count++];
    p.header = static_cast<unsigned>(reinterpret_cast<unsigned long>(header));
    p.has_position = position != nullptr;
    if (position != nullptr && position != delaySoundList + 36 * 2 + 4 && position != delaySoundList + 36 * 5 + 4) p.has_position = false;
    p.gain = gain; p.scale = scale; p.delay = delay; p.sequence = sequenceInProgress;
    return 0;
}
int sceSdRemote(int one, int command, int argument, int extra) {
    if (one != 1 || command != 0x8040 || extra != 0) return -1;
    remote_calls += 1;
    return remote_words[argument - 0x1700];
}
void soundFrame__Fv();
}
static void set_word(unsigned char* at, unsigned v) { std::memcpy(at, &v, 4); }
static void set_float(unsigned char* at, float v) { std::memcpy(at, &v, 4); }
static unsigned half(const unsigned char* at) { return at[0] | (at[1] << 8); }
int main() {
    // frame 8: not 0 mod 4? 8 & 3 == 0 -> screaming. entries 2 and 5 due, 7 not due.
    unsigned char* e = delaySoundList + 36 * 2;
    set_word(e, 0x1111); set_float(e + 16, 0.5f); set_float(e + 20, 0.75f); set_word(e + 24, 8); set_word(e + 28, 9); e[32] = 1;
    e = delaySoundList + 36 * 5;
    set_word(e, 0x2222); set_float(e + 16, 1.5f); set_float(e + 20, 2.5f); set_word(e + 24, 3); set_word(e + 28, 12); e[32] = 0;
    e = delaySoundList + 36 * 7;
    set_word(e, 0x3333); set_word(e + 24, 9); set_word(e + 28, 1);
    status_result = 0x20;
    soundFrame__Fv();
    if (std::strcmp(log_text, "dpsqaf") != 0) return 1;
    if (play_count != 2) return 2;
    if (plays[0].header != 0x1111 || !plays[0].has_position || plays[0].gain != 0.5f || plays[0].scale != 0.75f || plays[0].delay != 0.0f || plays[0].sequence != 9) return 3;
    if (plays[1].header != 0x2222 || plays[1].has_position || plays[1].gain != 1.5f || plays[1].scale != 2.5f || plays[1].sequence != 12) return 4;
    if (sequenceInProgress != -1) return 5;
    if (half(delaySoundList + 36 * 2 + 24) != 0 || half(delaySoundList + 36 * 5 + 24) != 0 || half(delaySoundList + 36 * 7 + 24) != 9) return 6;
    if (soundFrameNm != 9 || remote_calls != 2) return 7;
    if (stream_volume_calls != 0 || disableSoundPlaying != 2) return 8;
    // low bits of the incremented count are not 1 next time; unaffected channel flags
    // frame 9 -> 10: no screaming, no remote
    log_length = 0; log_text[0] = 0; remote_calls = 0;
    status_result = 0;
    soundFrame__Fv();
    if (std::strcmp(log_text, "dpqaf") != 0 || remote_calls != 0 || soundFrameNm != 10) return 9;
    if (stream_volume_calls != 1 || stream_volume != 0.5f * 0.78125f) return 10;
    if (disableSoundPlaying != 1) return 11;
    // channel clearing: soundFrameNm 16 -> 17 triggers remote and, 17 & 15 != 0
    soundFrameNm = 16;
    norrath_sound_channel_voice_map_view[0] = 1; norrath_sound_channel_voice_map_view[1] = 3;
    norrath_sound_channel_voice_map_view[2] = 0; norrath_sound_channel_voice_map_view[3] = 5;
    norrath_sound_channel_voice_map_view[4] = 0; norrath_sound_channel_voice_map_view[5] = 6;
    remote_words[0] = 1 << 5; remote_words[1] = 1 << 3;
    norrath_sound_channel_records_view[4] = 0x0f;
    norrath_sound_channel_records_view[20 + 4] = 0x1f;
    norrath_sound_channel_records_view[40 + 4] = 0x0f;
    soundMusicVolume = 0.9f;
    status_result = 0;
    soundFrame__Fv();
    if (half(norrath_sound_channel_records_view + 4) != 0x0b) return 12;
    if (half(norrath_sound_channel_records_view + 24) != 0x1f) return 13;
    if (half(norrath_sound_channel_records_view + 44) != 0x0f) return 14;
    if (stream_volume != 1.0f * 0.78125f) return 15;
    if (disableSoundPlaying != 0) return 16;
    // frame 31 -> 32: low four bits zero updates adjustments
    soundFrameNm = 31; adjust_calls = 0;
    soundFrame__Fv();
    if (adjust_calls != 1) return 17;
    soundFrameNm = 32; adjust_calls = 0;
    soundFrame__Fv();
    if (adjust_calls != 0) return 18;
    std::puts("sound frame contract passed");
    return 0;
}
