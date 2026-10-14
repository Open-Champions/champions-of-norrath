#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static int seen_flag;
static short seen_type;
static const void* seen_point;
static unsigned char seen_bytes[12];
extern "C" unsigned norrath_construct_Vehicle(void* object, const void* point, short type, char** arguments, int flag) {
    (void)object; (void)arguments;
    expect(stage == 0, "vehicle first");
    stage = 1;
    seen_flag = flag;
    seen_type = type;
    seen_point = point;
    std::memcpy(seen_bytes, point, 12);
    return 0x1u;
}
extern "C" unsigned norrath_vehicle_reset(void* object, int value) {
    expect(stage == 1 && value == 0, "reset after vtable store");
    stage = 2;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    unsigned stored = unsigned(raw[72]) | (unsigned(raw[73]) << 8) | (unsigned(raw[74]) << 16) | (unsigned(raw[75]) << 24);
    expect(stored == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_b_view_450250)),
           "vtable stored before reset");
    return 0x909u;
}
unsigned char norrath_actors_b_view_450250[4];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int main() {
    alignas(16) unsigned char object[80];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    std::memset(object, 0x5a, sizeof object);
    const unsigned result = norrath_construct_LavaTractor(object, point, static_cast<short>(-9), args);
    expect(result == 0x909u && stage == 2 && seen_flag == 0 && seen_type == -9, "vehicle flag and reset result");
    expect(seen_point != point && std::memcmp(seen_bytes, point, 12) == 0, "point snapshot");
    expect(word_at(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_b_view_450250)),
           "vtable before reset");
    if (fails != 0) return 1;
    std::puts("lava tractor constructor passed");
    return 0;
}
