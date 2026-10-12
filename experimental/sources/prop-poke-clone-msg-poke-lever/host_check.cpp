#define NORRATH_VSLOTS_B_HOST 1
#include "msg_poke.cpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static int rand_value = 0;
static int order = 0;
static int rand_order = 0, format_order = 0, lump_order = 0, play_order = 0, run_order = 0;
static void* rand_object = nullptr;
static char formatted[64];
static char lump_name[32];
static char lump_entry[64];
static char header_storage[8];
static void* play_header = nullptr;
static void* play_object = nullptr;
static float play_rate = 0.0f;
static void* run_object = nullptr;
static unsigned char* watched = nullptr;
static unsigned flag_at_rand = 0;
extern "C" int eRand__10GameObject(void* object) {
    rand_object = object; rand_order = ++order;
    flag_at_rand = vslots_b::read_word(vslots_b::address(object) + 12u);
    return rand_value;
}
extern "C" int sprintf(char* destination, const char* format, ...) {
    va_list args; va_start(args, format);
    format_order = ++order;
    std::strcpy(lump_entry, format);
    int n = std::vsnprintf(destination, 32, format, args);
    std::strcpy(formatted, destination);
    va_end(args);
    return n;
}
extern "C" char* lumpFindResource__FPcT0(char* lump, char* name) {
    lump_order = ++order;
    std::strcpy(lump_name, lump);
    std::strcpy(lump_entry, name);
    return header_storage;
}
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate) {
    play_order = ++order;
    play_header = header; play_object = object; play_rate = rate;
    return 0;
}
extern "C" unsigned norrath_actor_add_run_list(void* object) { run_order = ++order; run_object = object; return 0; }
int main() {
    unsigned char object[96];
    for (int variant = 0; variant < 4; ++variant) {
        std::memset(object, 0x5a, sizeof object);
        vslots_b::write_word(vslots_b::address(object) + 12u, 0xffffffffu);
        object[81] = 0;
        order = 0;
        rand_value = variant == 0 ? 0 : variant == 1 ? 1 : variant == 2 ? 6 : 7;
        msg_poke__5Leveri(object, 123);
        const char* expected = (rand_value & 1) ? "game_leverpull2.anm" : "game_leverpull1.anm";
        expect(std::strcmp(formatted, expected) == 0, "name from the low bit of eRand plus one");
        expect(std::strcmp(lump_name, "fx.lmp") == 0 && std::strcmp(lump_entry, expected) == 0, "lump fx.lmp, entry by name");
        expect(rand_object == object && play_object == object && run_object == object, "all on the object");
        expect(play_header == header_storage && play_rate == 1.0f, "sequence from lookup at rate 1.0");
        expect(vslots_b::read_word(vslots_b::address(object) + 12u) == 0xffefffffu, "bit 20 cleared, others kept");
        expect(flag_at_rand == 0xffefffffu, "flag cleared before eRand");
        expect(object[81] == 1, "byte 81 set");
        expect(object[80] == 0x5a && object[82] == 0x5a && object[11] == 0x5a && object[16] == 0x5a, "neighbours untouched");
        expect(rand_order < format_order && format_order < lump_order && lump_order < play_order && play_order < run_order, "call order");
    }
    return failures ? 1 : 0;
}
