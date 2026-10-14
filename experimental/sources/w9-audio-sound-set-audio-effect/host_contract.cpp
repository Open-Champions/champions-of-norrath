#define NORRATH_HOST_CONTRACT 1
#include "sound_set_audio_effect.cpp"
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
extern "C" int effectsBufferSize = 7;
extern "C" int nmSounds = 0;
extern "C" int effectModeSize[10] = {128, 9920, 8000, 18496, 28640, 44512, 63168, 98368, 98368, 15360};
extern "C" unsigned char soundList[16 * 4] = {};
extern "C" unsigned char norrath_sound_rpc_client_view[4] = {}, norrath_sound_rpc_send_view[16] = {}, norrath_sound_rpc_receive_view[64] = {};
static unsigned char headers[4][64];
extern "C" void* norrath_host_pointer(unsigned address) { return headers[address]; }   // addresses are small fake ids
static unsigned char heap_block[20];
static int events;
struct Event { int kind; int a, b, c, d, e; };
static Event log_[64];
static int remote_results[4];     // results of the 0x8190 / 0x80f0 calls in order
static int result_index;
extern "C" void* memAlloc__FiPc(int size, char* name) { log_[events++] = {1, size, name[0], 0, 0, 0}; std::memset(heap_block, 0xcc, 20); return heap_block; }
extern "C" void memFree__FPv(void* p) { log_[events++] = {2, p == heap_block, 0, 0, 0, 0}; }
extern "C" int norrath_sound_call_rpc(void* client, int command, int mode, void* send, int sb, void* recv, int rb, void* ef, void* ep) {
    log_[events++] = {3, command, mode, sb, rb, client == norrath_sound_rpc_client_view && send == norrath_sound_rpc_send_view && recv == norrath_sound_rpc_receive_view && !ef && !ep};
    return 0;
}
extern "C" int sceSdRemote(int one, ...) {
    va_list ap; va_start(ap, one);
    int a = va_arg(ap, int), b = va_arg(ap, int), c = va_arg(ap, int), d = va_arg(ap, int);
    va_end(ap);
    log_[events++] = {4, one, a, b, c, d};
    if (a == 0x8190 || a == 0x80f0) return remote_results[result_index++];
    return 99;
}
static int count_kind(int k) { int n = 0; for (int i = 0; i < events; ++i) n += log_[i].kind == k; return n; }
static void reset() { events = 0; result_index = 0; }
int main() {
    // table size is read with the raw argument; limits apply afterwards
    remote_results[0] = 0; remote_results[1] = 1;
    soundSetAudioEffect__Fiiiii(3, -40000, 40000, -5, 500);
    if (effectsBufferSize != 18496) return 1;
    int block_mode, c, d; short s8, s10;
    std::memcpy(&block_mode, heap_block + 4, 4); std::memcpy(&s8, heap_block + 8, 2); std::memcpy(&s10, heap_block + 10, 2);
    std::memcpy(&c, heap_block + 12, 4); std::memcpy(&d, heap_block + 16, 4);
    if (block_mode != 3 || s8 != -32768 || s10 != 32767 || c != 0 || d != 127) return 2;
    // sequence of events: rpc 176, memAlloc, 0x8050, 0x8190, 0x80f0, 0x81a0 ... memFree, rpc 192
    if (log_[0].kind != 3 || log_[0].a != 176 || log_[0].b != 0 || log_[0].c != 16 || log_[0].d != 64 || !log_[0].e) return 3;
    if (log_[1].kind != 1 || log_[1].a != 20 || log_[1].b != 0) return 4;
    if (log_[2].kind != 4 || log_[2].b != 0x8050 || log_[2].c != 0x1d01 || log_[2].d != 0x1fffff) return 5;
    if (log_[3].b != 0x8190 || log_[3].c != 1 || log_[3].d != 0 || log_[3].e != 3) return 6;
    if (log_[4].b != 0x80f0 || log_[4].c != 0 || log_[4].d != 1) return 7;
    if (log_[5].b != 0x81a0 || log_[5].c != 1 || log_[5].d != static_cast<int>(reinterpret_cast<std::uintptr_t>(heap_block))) return 8;
    if (count_kind(4) != 1 + 2 + 2 + 8 + 2 + 1) return 9;   // 0x8050, one pass, two block calls, eight 0x8030, two 0x8010, one 0x8070
    const Event& last = log_[events - 1];
    if (last.kind != 3 || last.a != 192) return 10;
    if (log_[events - 2].kind != 2 || !log_[events - 2].a) return 11;
    if (log_[events - 3].b != 0x8070 || log_[events - 3].c != 3 || log_[events - 3].d != 1) return 12;
    if (log_[9].b != 0x8030 || log_[9].c != 0x1900 || log_[10].d != 0) return 13;
    if (log_[12].b != 0x8030 || log_[12].c != 0x1a01 || log_[12].d != 0xffffff || log_[13].d != 0x7ffff) return 14;
    // a failing first pass runs a second pass (the first call is nonzero, or the second is not 1)
    reset(); remote_results[0] = 5; remote_results[1] = 1; remote_results[2] = 0; remote_results[3] = 1;
    soundSetAudioEffect__Fiiiii(9, 0, 0, 200, -1);
    if (count_kind(4) != 1 + 4 + 2 + 8 + 2 + 1) return 20;
    std::memcpy(&block_mode, heap_block + 4, 4); std::memcpy(&c, heap_block + 12, 4); std::memcpy(&d, heap_block + 16, 4);
    if (block_mode != 9 || c != 127 || d != 0) return 21;
    if (log_[4].c != 0 || log_[5].b != 0x8190 || log_[5].d != 1 || log_[6].b != 0x80f0 || log_[6].c != 1) return 22;
    reset(); remote_results[0] = 0; remote_results[1] = 2; remote_results[2] = 0; remote_results[3] = 2;
    soundSetAudioEffect__Fiiiii(0, 0, 0, 0, 0);          // two passes fail; the loop ends after pass 1
    if (count_kind(4) != 1 + 4 + 2 + 8 + 2 + 1) return 23;
    const int second_results[6] = {0, -202, -5, 0, 1, 7};   // only a second result of 1 passes
    const bool second_fails[6] = {true, true, true, true, false, true};
    for (int k = 0; k < 6; ++k) {
        reset(); remote_results[0] = 0; remote_results[1] = second_results[k]; remote_results[2] = 0; remote_results[3] = 1;
        soundSetAudioEffect__Fiiiii(0, 0, 0, 0, 0);
        if (count_kind(4) != 1 + (second_fails[k] ? 4 : 2) + 2 + 8 + 2 + 1) return 30 + k;
    }
    reset(); remote_results[0] = 3; remote_results[1] = 1; remote_results[2] = 0; remote_results[3] = 1;   // first result nonzero fails
    soundSetAudioEffect__Fiiiii(0, 0, 0, 0, 0);
    if (count_kind(4) != 1 + 4 + 2 + 8 + 2 + 1) return 40;
    // the sound list: effect 2 gives 8000 bytes, so the low-water mark is 0x1fffff - 8000
    reset(); remote_results[0] = 0; remote_results[1] = 1;
    const int low_water = 0x1fffff - 8000;
    nmSounds = 4;
    const int header_word[4] = {100, 0x1fffff, 0x200000, 100};
    const int size[4] = {low_water, low_water, low_water, low_water - 1};
    for (unsigned i = 0; i < 4; ++i) {
        std::memcpy(soundList + i * 16 + 4, &i, 4);                  // header "address" is the index
        std::memcpy(soundList + i * 16 + 8, &size[i], 4);
        std::memcpy(headers[i] + 36, &header_word[i], 4);
    }
    soundSetAudioEffect__Fiiiii(2, 0, 0, 0, 0);
    int after[4];
    for (int i = 0; i < 4; ++i) std::memcpy(&after[i], headers[i] + 36, 4);
    if (after[0] != 0x5000 || after[1] != 0x5000 || after[2] != 0x200000 || after[3] != 100) return 24;
    if (effectsBufferSize != 8000) return 25;
    nmSounds = 0;
    std::puts("sound_set_audio_effect passed");
    return 0;
}
