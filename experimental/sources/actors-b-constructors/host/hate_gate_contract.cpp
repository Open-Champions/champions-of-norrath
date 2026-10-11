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
extern "C" unsigned norrath_construct_AutoProp(void* object, char* name, const void* point,
                                              short type, char** arguments) {
    (void)name; (void)arguments;
    ++calls;
    seen_point = point;
    seen_type = type;
    std::memcpy(seen_bytes, point, 12);
    auto* raw = reinterpret_cast<unsigned char*>(object);
    raw[208] = 0x20; raw[209] = 0; raw[210] = 0; raw[211] = 0;
    raw[72] = 0xee;
    return 0xb22u;
}
unsigned char norrath_actors_b_view_44f0a8[4];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int main() {
    alignas(16) unsigned char object[256];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char name[] = "gate";
    char* args[] = {nullptr};
    std::memset(object, 0x5a, sizeof object);
    const unsigned result = norrath_construct_HateGate(object, name, point, static_cast<short>(-1), args);
    expect(result == 0xb22u, "forwards AutoProp result");
    expect(calls == 1 && seen_point != point && std::memcmp(seen_bytes, point, 12) == 0, "point snapshot");
    expect(seen_type == -1, "short");
    expect(word_at(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_b_view_44f0a8)),
           "vtable");
    expect(word_at(object + 208) == 0x21u, "word 208 ored after AutoProp");
    expect(word_at(object + 12) == 0x5a5a5a5au, "object+12 left unchanged");
    if (fails != 0) return 1;
    std::puts("hate gate constructor passed");
    return 0;
}
