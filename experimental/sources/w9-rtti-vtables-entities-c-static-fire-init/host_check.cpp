// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "init.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
static char log_text[16];
static unsigned log_n = 0;
static unsigned roll_value = 0;
static int wind_arg = -999, alloc_size = 0;
static void* seen[4];
static char alloc_name[16];
static unsigned char heap[16];
static unsigned flags_at_wind = 0, field56_at_wind = 0xffff;
static void note(char c) { log_text[log_n++] = c; log_text[log_n] = 0; }
extern "C" unsigned eRand__10GameObject(void* o) { note('r'); seen[0] = o; return roll_value; }
extern "C" void newWind__10StaticFirei(void* o, int v) {
    note('w'); seen[1] = o; wind_arg = v;
    flags_at_wind = rtti_c::read_word(rtti_c::address(o) + 12u);
    field56_at_wind = rtti_c::read_u16(rtti_c::address(o) + 56u);
}
extern "C" void objectAddToRunList__FP10GameObject(void* o) { note('a'); seen[2] = o; }
extern "C" void objectAlwaysDraw__FP10GameObject(void* o) { note('d'); seen[3] = o; }
extern "C" void* memAlloc__FiPc(int size, const char* name) {
    note('m'); alloc_size = size; std::strncpy(alloc_name, name, 15); return heap;
}
int main() {
    int bad = 0;
    for (unsigned rolls : {0u, 1u, 299u, 300u, 12345u, 0xffffffffu, 0x80000000u}) {
        for (unsigned b111 : {0u, 1u, 0x80u}) {
            for (unsigned b112 : {0u, 1u, 0xffu}) {
                unsigned char object[1300];
                std::memset(object, 0x5a, sizeof object);
                object[111] = static_cast<unsigned char>(b111);
                object[112] = static_cast<unsigned char>(b112);
                rtti_c::write_word(rtti_c::address(object) + 12u, 0x00800000u);
                roll_value = rolls; log_n = 0; log_text[0] = 0; wind_arg = -999; alloc_size = 0; alloc_name[0] = 0;
                rtti_c::host_note(heap); rtti_c::host_note(object + 188);
                norrath_static_fire_init(object);
                const char* want = b111 ? "rwadm" : "rwad";
                if (std::strcmp(log_text, want) != 0) { std::printf("order %s\n", log_text); ++bad; }
                for (void* s : seen) if (s != object) { std::printf("callee object\n"); ++bad; }
                if (wind_arg != 0 || field56_at_wind != 0u || flags_at_wind != 0x00840001u) { std::printf("wind state\n"); ++bad; }
                const rtti_c::Address a = rtti_c::address(object);
                float expect_roll = static_cast<float>(rolls % 300u);
                unsigned eb; std::memcpy(&eb, &expect_roll, 4);
                if (rtti_c::read_word(a + 76u) != eb) { std::printf("+76\n"); ++bad; }
                if (rtti_c::read_word(a + 28u) != 0x41800000u || rtti_c::read_u16(a + 56u) != 0u || object[40] != 60 ||
                    rtti_c::read_u16(a + 120u) != 0u || rtti_c::read_u16(a + 122u) != 0u ||
                    rtti_c::read_word(a + 104u) != 0x46c35000u || rtti_c::read_word(a + 124u) != 0xffffffffu ||
                    rtti_c::read_word(a + 84u) != 0x3f800000u || rtti_c::read_word(a + 100u) != 0x3f800000u || object[108] != 0) { std::printf("constants\n"); ++bad; }
                if (b111) {
                    if (alloc_size != 2500 || std::strcmp(alloc_name, "smokebomb") != 0) { std::printf("alloc args\n"); ++bad; }
                    if (rtti_c::read_u16(a + 118u) != 250u || rtti_c::read_word(a + 1188u) != rtti_c::address_bits(heap) || rtti_c::read_word(a + 80u) != 0x44160000u) { std::printf("alloc path\n"); ++bad; }
                } else {
                    const unsigned want80 = b112 ? 0x43700000u : 0x43f00000u;
                    if (rtti_c::read_u16(a + 118u) != 100u || rtti_c::read_word(a + 1188u) != rtti_c::address_bits(object + 188) || rtti_c::read_word(a + 80u) != want80) { std::printf("inline path b112 %u\n", b112); ++bad; }
                }
            }
        }
    }
    std::printf(bad ? "failed\n" : "norrath_static_fire_init passed\n");
    return bad != 0;
}
