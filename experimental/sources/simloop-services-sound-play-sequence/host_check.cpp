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
static int order = 0;
static int add_at = 0, destroy_at = 0, link_at = 0, paused_at = 0;
static void* add_state;
static void* add_header;
static int add_flags;
static void* destroyed;
static void* linked[3];
static int paused_result = 1;
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags) {
    add_at = ++order; add_state = state; add_header = header; add_flags = flags;
}
extern "C" void listDestroy__FP8ListHead(void* head) { destroy_at = ++order; destroyed = head; }
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* object) {
    link_at = ++order; linked[0] = head; linked[1] = link; linked[2] = object;
}
extern "C" int gameIsPaused__Fv() { paused_at = ++order; return paused_result; }
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(unsigned char*, unsigned char*, float);
static void set_word(unsigned char* at, unsigned value) { std::memcpy(at, &value, 4); }
static unsigned get_word(const unsigned char* at) { unsigned v; std::memcpy(&v, at, 4); return v; }
static unsigned low(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(pointer)); }
static unsigned char* rec(int index) { return norrath_sound_sequence_records_view + index * 76; }
int main() {
    unsigned char* page = static_cast<unsigned char*>(mmap(nullptr, 0x1000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0));
    if (page == MAP_FAILED) return 90;
    if ((reinterpret_cast<uintptr_t>(page) >> 32) != ((reinterpret_cast<uintptr_t>(page) + 0xfff) >> 32)) return 91;
    pointer_high = reinterpret_cast<uintptr_t>(page) & ~uintptr_t{0xffffffffu};
    unsigned char* header = page;
    unsigned char* other = page + 0x100;
    unsigned char* object = page + 0x200;
    for (int i = 0; i < 300; ++i) norrath_sound_group_limits_view[i] = 2;
    norrath_sound_group_limits_view[7] = 2;
    if (soundPlaySequence__FP15AnimationHeaderP10GameObjectf(nullptr, nullptr, 1.0f) != -1 || order != 0) return 1;

    header[60] = 7; other[60] = 7;
    for (int i = 0; i < 32; ++i) { rec(i)[4] = 1; set_word(rec(i) + 8, low(other)); }
    rec(3)[4] = 0;
    rec(9)[4] = 0;
    rec(9)[10] = 0xaa;
    set_word(rec(9) + 8, 0);
    set_word(rec(3) + 8, 0);
    // limit 2 and 30 matching records reject even though slots are free
    if (soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, nullptr, 1.0f) != -1 || order != 0) return 2;
    // group zero skips the limit and takes the last free slot
    header[60] = 0; other[60] = 0;
    norrath_sound_sequence_id_spinner_view[0] = 127;
    for (int i = 0; i < 20; ++i) object[16 + i] = static_cast<unsigned char>(0x30 + i);
    int got = soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, object, 2.5f);
    if (got != (9 | (127 << 8) | 0x8000)) return 3;
    unsigned char* r = rec(9);
    if (add_state != r || add_header != header || add_flags != 4) return 4;
    if (destroyed != r + 40 || linked[0] != r + 40 || linked[1] != object || linked[2] != object) return 5;
    if (!(add_at < destroy_at && destroy_at < link_at && link_at < paused_at)) return 6;
    if (std::memcmp(r + 44, object + 16, 12) != 0 || r[58] != 0) return 7;
    if (r[56] != 127 || r[57] != 0 || r[59] != 1 || r[60] != 0 || r[6] != 0 || r[7] != 0) return 8;
    if (norrath_sound_sequence_id_spinner_view[0] != 0 || norrath_sound_sequence_id_spinner_view[1] != 0) return 9;
    float gain; unsigned bitsv = get_word(r + 64); std::memcpy(&gain, &bitsv, 4);
    if (gain != 2.5f || get_word(r + 68) != 0x43b40000u || get_word(r + 72) != 0x44520000u) return 10;
    // null object
    rec(9)[4] = 0;
    got = soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, nullptr, 1.0f);
    if (got != (9 | 0x8000) || rec(9)[58] != 1 || norrath_sound_sequence_id_spinner_view[0] != 1) return 11;
    // nothing free
    for (int i = 0; i < 32; ++i) { rec(i)[4] = 1; set_word(rec(i) + 8, low(other)); }
    if (soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, nullptr, 1.0f) != -1) return 12;
    std::puts("play sequence contract passed");
    return 0;
}
