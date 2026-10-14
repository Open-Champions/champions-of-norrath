#include "sound_clear.cpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
extern "C" unsigned char delaySoundList[1152] = {};
extern "C" unsigned char paused = 1;
extern "C" int nmSounds = 0;
extern "C" unsigned char soundList[16 * 6] = {};
extern "C" unsigned char sequence[2432] = {};
extern "C" unsigned char norrath_sound_channel_records_view[43 * 20 + 4] = {};
extern "C" unsigned char norrath_sound_channel_voice_map_view[86] = {};
extern "C" unsigned char norrath_sound_key_on_words_view[8] = {};
extern "C" unsigned char norrath_sound_key_off_words_view[8] = {};
extern "C" unsigned char norrath_sound_rpc_client_view[96] = {};
extern "C" unsigned char norrath_sound_rpc_send_view[64] = {};
extern "C" unsigned char norrath_sound_rpc_receive_view[64] = {};
static std::vector<std::string> ev;
static std::vector<int> stops, rpc_values;
static std::vector<void*> destroyed;
extern "C" void soundStopSound__Fi(int id) { stops.push_back(id); ev.push_back("stop"); }
extern "C" void listDestroy__FP8ListHead(void* h) { destroyed.push_back(h); ev.push_back("destroy"); }
extern "C" void soundFadeStream__Fi(int s) { ev.push_back("fade" + std::to_string(s)); }
extern "C" void soundSetAudioEffect__Fiiiii(int a, int b, int c, int d, int e) {
    if (a | b | c | d | e) std::exit(90);
    ev.push_back("effect");
}
extern "C" void soundFlushCommandBuffer__Fv() { ev.push_back("flush"); }
extern "C" int norrath_sound_call_rpc(void* c, int cmd, int mode, void* s, int sl, void* r, int rl, void* e, void* p) {
    if (c != norrath_sound_rpc_client_view || cmd != 112 || mode != 0 || s != norrath_sound_rpc_send_view ||
        sl != 16 || r != norrath_sound_rpc_receive_view || rl != 64 || e || p) std::exit(91);
    int w[2]; std::memcpy(w, norrath_sound_rpc_send_view, 8);
    if (w[1] != 0) std::exit(92);
    rpc_values.push_back(w[0]); ev.push_back("rpc"); return 0;
}
static unsigned w(const unsigned char* a) { unsigned v; std::memcpy(&v, a, 4); return v; }
int main() {
    std::memset(delaySoundList, 0x77, sizeof delaySoundList);
    // channel c uses word c % 2 and bit c; the last channel uses a byte above 31 to show the wrap
    for (int c = 0; c < 43; ++c) {
        norrath_sound_channel_voice_map_view[c * 2] = c % 2;
        norrath_sound_channel_voice_map_view[c * 2 + 1] = c % 32;
        w(norrath_sound_channel_records_view + c * 20);
        std::memset(norrath_sound_channel_records_view + c * 20, 0xee, 20);
    }
    std::memset(norrath_sound_key_on_words_view, 0xff, 8);
    std::memset(norrath_sound_key_off_words_view, 0, 8);
    norrath_sound_channel_records_view[43 * 20] = 0x5a;
    // sounds: locks 5, 4, 3, 9; level 3 keeps the first two
    nmSounds = 4;
    soundList[12] = 5; soundList[28] = 4; soundList[44] = 3; soundList[60] = 9;
    // sequences: 3 and 31 active
    sequence[3 * 76 + 4] = 1; sequence[3 * 76 + 56] = 0x34; sequence[3 * 76 + 57] = 0x12;
    sequence[31 * 76 + 4] = 0xff; 
    soundClear__Fi(3);
    for (unsigned char b : delaySoundList) if (b != 0) return 1;
    if (paused != 0) return 2;
    for (int c = 0; c < 43; ++c) {
        if (w(norrath_sound_channel_records_view + c * 20) != 0) return 3;
        if (norrath_sound_channel_records_view[c * 20 + 4] != 0 || norrath_sound_channel_records_view[c * 20 + 5] != 0) return 4;
        if (norrath_sound_channel_records_view[c * 20 + 6] != 0xee || norrath_sound_channel_records_view[c * 20 + 19] != 0xee) return 5;
    }
    if (norrath_sound_channel_records_view[43 * 20] != 0x5a) return 6;
    // word 0 holds even channels 0,2,..,42 : bits c%32 ; word 1 odd channels 1..41
    unsigned off0 = 0, off1 = 0;
    for (int c = 0; c < 43; ++c) (c % 2 ? off1 : off0) |= 1u << (c % 32);
    if (w(norrath_sound_key_off_words_view) != off0 || w(norrath_sound_key_off_words_view + 4) != off1) return 7;
    if (w(norrath_sound_key_on_words_view) != ~off0 || w(norrath_sound_key_on_words_view + 4) != ~off1) return 8;
    if (nmSounds != 2) return 9;
    if (stops != std::vector<int>{3 | (0x1234 << 8) | 0x8000, 31 | 0x8000}) return 10;
    if (destroyed.size() != 32) return 11;
    for (int i = 0; i < 32; ++i) if (destroyed[i] != sequence + i * 76 + 40) return 12;
    if (rpc_values != std::vector<int>{0, 1, 2}) return 13;
    // order: per-sequence stop precedes its destroy; fades after the RPCs, flush last
    if (ev.back() != "flush" || ev[ev.size() - 2] != "effect" || ev[ev.size() - 3] != "fade2") return 14;
    if (ev[0] != "destroy" || ev[1] != "destroy" || ev[2] != "destroy" || ev[3] != "stop" || ev[4] != "destroy") return 15;
    // lock levels: all above -> all kept; a nonpositive count -> 0; equal lock stops the scan
    nmSounds = 4; soundList[12] = 5; soundList[28] = 5; soundList[44] = 5; soundList[60] = 5;
    soundClear__Fi(4);
    if (nmSounds != 4) return 16;
    soundClear__Fi(5);
    if (nmSounds != 0) return 17;
    nmSounds = -3;
    soundClear__Fi(-1);
    if (nmSounds != 0) return 18;
    nmSounds = 3; soundList[12] = static_cast<unsigned char>(-1); soundList[28] = 0;
    soundList[44] = static_cast<unsigned char>(-3);
    soundClear__Fi(-2);
    if (nmSounds != 2) return 19;       // signed bytes: -1 and 0 exceed -2, -3 does not
    std::puts("sound_clear passed");
    return 0;
}
