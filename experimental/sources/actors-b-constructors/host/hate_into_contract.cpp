#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static void* seen_state;
static void* seen_header;
static int seen_flags;
static unsigned char header_obj[8];
extern "C" unsigned norrath_construct_AutoProp(void* object, char* name, const void* point,
                                              short type, char** arguments) {
    (void)name; (void)point; (void)type; (void)arguments;
    expect(stage == 0, "AutoProp first");
    stage = 1;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    const unsigned bits = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(header_obj));
    raw[296] = static_cast<unsigned char>(bits);
    raw[297] = static_cast<unsigned char>(bits >> 8);
    raw[298] = static_cast<unsigned char>(bits >> 16);
    raw[299] = static_cast<unsigned char>(bits >> 24);
    raw[156] = 0x7f;
    raw[208] = 0x40;
    return 0x111u;
}
extern "C" unsigned norrath_actor_animation_add(void* state, void* header, int flags) {
    expect(stage == 1, "animAdd after AutoProp");
    stage = 2;
    seen_state = state;
    seen_header = header;
    seen_flags = flags;
    auto* raw = reinterpret_cast<unsigned char*>(state) - 152;
    expect(raw[156] == 0, "byte 156 cleared before animAdd");
    const unsigned stored = unsigned(raw[72]) | (unsigned(raw[73]) << 8) | (unsigned(raw[74]) << 16) | (unsigned(raw[75]) << 24);
    expect(stored == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_b_view_44f150)),
           "vtable stored before animAdd");
    raw[208] = 0x40; raw[209] = 0; raw[210] = 0; raw[211] = 0;
    return 0xd44u;
}
unsigned char norrath_actors_b_view_44f150[4];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int main() {
    alignas(16) unsigned char object[320];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char name[] = "into";
    char* args[] = {nullptr};
    std::memset(object, 0x5a, sizeof object);
    const unsigned result = norrath_construct_HateInto(object, name, point, 3, args);
    expect(result == 0xd44u, "forwards animAdd result");
    expect(stage == 2, "both calls");
    const auto header_bits = static_cast<std::uintptr_t>(static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(header_obj)));
    expect(seen_state == object + 152 && seen_header == reinterpret_cast<void*>(header_bits) && seen_flags == 1,
           "animAdd arguments");
    expect(word_at(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_b_view_44f150)),
           "vtable");
    expect(word_at(object + 208) == 0x41u, "word 208 ored after animAdd");
    if (fails != 0) return 1;
    std::puts("hate into constructor passed");
    return 0;
}
