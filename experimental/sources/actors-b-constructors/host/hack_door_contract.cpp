#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int calls;
static const void* seen_point;
static unsigned char seen_bytes[12];
static short seen_type;
static char** seen_args;
static char* seen_name;
static void* seen_object;
extern "C" unsigned norrath_construct_AutoProp(void* object, char* name, const void* point,
                                              short type, char** arguments) {
    ++calls;
    seen_object = object;
    seen_name = name;
    seen_point = point;
    seen_type = type;
    seen_args = arguments;
    std::memcpy(seen_bytes, point, 12);
    auto* raw = reinterpret_cast<unsigned char*>(object);
    raw[12] = 0x10; raw[13] = 0; raw[14] = 0; raw[15] = 0;
    raw[72] = 0xee;
    return 0xa11u;
}
unsigned char norrath_actors_b_view_44ee08[4];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int main() {
    alignas(16) unsigned char object[128];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char name[] = "hack";
    char* args[] = {name, nullptr};
    std::memset(object, 0x5a, sizeof object);
    const unsigned result = norrath_construct_HackDoor(object, name, point, static_cast<short>(-2), args);
    expect(result == 0xa11u, "forwards AutoProp result");
    expect(calls == 1, "one AutoProp call");
    expect(seen_object == object && seen_name == name && seen_args == args, "object name and arguments");
    expect(seen_point != point && std::memcmp(seen_bytes, point, 12) == 0, "point snapshot");
    expect(seen_type == -2, "sign-extended short");
    expect(word_at(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_b_view_44ee08)),
           "vtable after AutoProp");
    expect(word_at(object + 12) == 0x11u, "flags ored after AutoProp");
    unsigned char overlap[32];
    std::memset(overlap, 0x3c, sizeof overlap);
    const unsigned char pattern[12] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 11, 10};
    std::memcpy(overlap + 16, pattern, 12);
    calls = 0;
    (void)norrath_construct_HackDoor(overlap, name, overlap + 16, static_cast<short>(0x7fff), args);
    expect(calls == 1 && seen_point != overlap + 16 && std::memcmp(seen_bytes, pattern, 12) == 0,
           "overlapping point is snapshotted");
    expect(seen_type == 0x7fff, "positive short preserved");
    if (fails != 0) return 1;
    std::puts("hack door constructor passed");
    return 0;
}
