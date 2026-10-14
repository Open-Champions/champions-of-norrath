#include "play_sequence_at_point.cpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <sys/mman.h>
extern "C" unsigned char norrath_sound_sequence_records_view[2432] = {};
extern "C" unsigned char norrath_sound_group_limits_view[300] = {};
extern "C" unsigned char norrath_sound_sequence_id_spinner_view[2] = {};
static uintptr_t pointer_high = 0;
extern "C" void* norrath_host_pointer(unsigned address) {
    return address == 0 ? nullptr : reinterpret_cast<void*>(pointer_high | address);
}
static int order = 0, add_at = 0, destroy_at = 0, paused_at = 0, add_flags = 0, paused_result = 1;
static void *add_state, *add_header, *destroyed;
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int f) {
    add_at = ++order; add_state = s; add_header = h; add_flags = f;
}
extern "C" void listDestroy__FP8ListHead(void* h) { destroy_at = ++order; destroyed = h; }
extern "C" int gameIsPaused__Fv() { paused_at = ++order; return paused_result; }
static void set_word(unsigned char* at, unsigned v) { std::memcpy(at, &v, 4); }
static unsigned get_word(const unsigned char* at) { unsigned v; std::memcpy(&v, at, 4); return v; }
static float get_float(const unsigned char* at) { float v; std::memcpy(&v, at, 4); return v; }
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
static unsigned char* rec(int i) { return norrath_sound_sequence_records_view + i * 76; }
int main() {
    unsigned char* page = static_cast<unsigned char*>(mmap(nullptr, 0x1000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0));
    if (page == MAP_FAILED) return 90;
    if ((reinterpret_cast<uintptr_t>(page) >> 32) != ((reinterpret_cast<uintptr_t>(page) + 0xfff) >> 32)) return 91;
    pointer_high = reinterpret_cast<uintptr_t>(page) & ~uintptr_t{0xffffffffu};
    unsigned char* header = page;
    unsigned char* other = page + 0x100;
    for (int i = 0; i < 300; ++i) norrath_sound_group_limits_view[i] = 2;
    Point3 p = {1.5f, -2.0f, 3.25f};
    if (soundPlaySequence__FP15AnimationHeaderG6Point3f(nullptr, p, 1.0f) != -1 || order != 0) return 1;
    // nonzero group: 30 matching records and a limit of 2 reject although slots are free
    header[60] = 7; other[60] = 7;
    for (int i = 0; i < 32; ++i) { rec(i)[4] = 1; set_word(rec(i) + 8, low(other)); }
    rec(3)[4] = 0; rec(9)[4] = 0; rec(9)[20] = 0xaa;
    set_word(rec(9) + 8, 0); set_word(rec(3) + 8, 0);
    if (soundPlaySequence__FP15AnimationHeaderG6Point3f(header, p, 1.0f) != -1 || order != 0) return 2;
    // group zero skips the limit and takes the last free record
    header[60] = 0; other[60] = 0;
    norrath_sound_sequence_id_spinner_view[0] = 127;
    int got = soundPlaySequence__FP15AnimationHeaderG6Point3f(header, p, 2.5f);
    if (got != (9 | (127 << 8) | 0x8000)) return 3;
    unsigned char* r = rec(9);
    if (add_state != r || add_header != header || add_flags != 4) return 4;
    if (destroyed != r + 40 || !(add_at < destroy_at && destroy_at < paused_at)) return 5;
    if (get_float(r + 44) != 1.5f || get_float(r + 48) != -2.0f || get_float(r + 52) != 3.25f) return 6;
    if (r[58] != 0 || r[59] != 1 || r[60] != 0 || r[6] != 0 || r[7] != 0) return 7;
    if (r[56] != 127 || r[57] != 0) return 8;
    if (norrath_sound_sequence_id_spinner_view[0] != 0 || norrath_sound_sequence_id_spinner_view[1] != 0) return 9;   // 128 & 0x7f
    if (get_float(r + 64) != 2.5f || get_word(r + 68) != 0x43b40000u || get_word(r + 72) != 0x44520000u) return 10;
    // neighbours of the chosen record are untouched
    if (rec(8)[56] != 0) return 111;
    if (rec(10)[56] != 0) return 112;
    if (r[20] != 0xaa) return 113;
    // the spinner counts up, a different group in the used records does not match
    rec(9)[4] = 0; header[60] = 5; paused_result = 0;
    got = soundPlaySequence__FP15AnimationHeaderG6Point3f(header, p, 1.0f);
    if (got != (9 | (0 << 8) | 0x8000) || r[59] != 0 || norrath_sound_sequence_id_spinner_view[0] != 1) return 12;
    // a limit of 2: the second matching record rejects, the first still allows
    rec(9)[4] = 0; other[60] = 5; header[60] = 5;
    for (int i = 0; i < 32; ++i) if (i != 9) { rec(i)[4] = 0; }
    rec(0)[4] = 1; set_word(rec(0) + 8, low(other));
    if (soundPlaySequence__FP15AnimationHeaderG6Point3f(header, p, 1.0f) < 0) return 13;   // one match < limit 2
    rec(1)[4] = 1; set_word(rec(1) + 8, low(other));
    if (soundPlaySequence__FP15AnimationHeaderG6Point3f(header, p, 1.0f) != -1) return 14; // second match reaches the limit
    // no free record at all
    for (int i = 0; i < 32; ++i) { rec(i)[4] = 1; set_word(rec(i) + 8, low(other)); }
    other[60] = 9; header[60] = 0;
    if (soundPlaySequence__FP15AnimationHeaderG6Point3f(header, p, 1.0f) != -1) return 15;
    std::puts("play_sequence_at_point passed");
    return 0;
}
