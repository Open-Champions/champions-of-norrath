#include "fire_bomblet_ctor.cpp"

#include <cstdio>
#include <cstring>
#include <vector>

unsigned char norrath_actors_a_view_438e98[32];
unsigned char vt_11FireBomblet[16] __asm__("_vt$11FireBomblet");

static std::vector<const char*> order;
static unsigned char* current;
static unsigned anchor_seen[3];
static void* map_seen;
static char** arguments_seen = reinterpret_cast<char**>(1);
static unsigned char light_at_add[64];
static unsigned char light_byte8_at_add;
static unsigned char byte156_at_run_list;
static void* run_list_object;

static unsigned word(const unsigned char* base, unsigned offset) { unsigned v; std::memcpy(&v, base + offset, 4); return v; }
static unsigned short half(const unsigned char* base, unsigned offset) { unsigned short v; std::memcpy(&v, base + offset, 2); return v; }
static float flt(const unsigned char* base, unsigned offset) { float v; std::memcpy(&v, base + offset, 4); return v; }
static float f(unsigned b) { float v; std::memcpy(&v, &b, 4); return v; }

extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments) {
    order.push_back("construct");
    std::memcpy(anchor_seen, point, 12);
    map_seen = resource_map;
    arguments_seen = arguments;
    // The base constructor places the anchor at +16 and leaves other bytes alone.
    std::memcpy(static_cast<unsigned char*>(object) + 16, point, 12);
    return 0;
}
extern "C" void norrath_light_add(void* light) {
    order.push_back("light");
    std::memcpy(light_at_add, light, sizeof light_at_add);
    light_byte8_at_add = static_cast<unsigned char*>(light)[8];
    if (light != current + 88) std::fprintf(stderr, "fail: light address\n");
}
extern "C" void objectAddToRunList__FP10GameObject(void* object) {
    order.push_back("runlist");
    run_list_object = object;
    byte156_at_run_list = static_cast<unsigned char*>(object)[156];
}


int main() {
    int bad = 0;
    auto expect = [&](bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); bad = 1; } };
    alignas(16) unsigned char object[192];
    current = object;
    const float P[3] = {100.0f, 250.0f, 40.0f};
    const float A[3] = {70.0f, 30.0f, 12.0f};
    const float V[3] = {1.5f, -2.5f, 9.0f};
    char marker[4] = "m";

    // Velocity supplied.
    std::memset(object, 0xcc, sizeof object);
    __11FireBombletG6Point3sPcT1P6Point3i(object, P, 0x1234, marker, A, V, 0x1ff);
    expect(order.size() == 3 && std::strcmp(order[0], "construct") == 0 && std::strcmp(order[1], "light") == 0 &&
               std::strcmp(order[2], "runlist") == 0, "call order");
    expect(std::memcmp(anchor_seen, A, 12) == 0 && map_seen == norrath_actors_a_view_438e98 && arguments_seen == nullptr,
           "base constructor arguments");
    expect(word(object, 72) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_11FireBomblet)), "vtable");
    expect(std::memcmp(object + 136, A, 12) == 0, "anchor copy at +136");
    const unsigned char* L = light_at_add;
    expect(L[9] == 0 && light_byte8_at_add == 0, "light flag bytes cleared before lightAdd");
    const unsigned short want_halves[8] = {255, 128, 0, 0xfff0, 255, 128, 0, 0xfff0};
    for (int i = 0; i < 8; ++i) expect(half(L, 10 + 2 * i) == want_halves[i], "light halfwords");
    expect(flt(L, 28) == 120.0f && flt(L, 36) == 120.0f && word(L, 32) == 0x3891a2b3u && word(L, 40) == 0x3891a2b3u &&
               word(L, 44) == 0u, "light floats");
    expect(object[40] == 12 && flt(object, 28) == 12.0f && word(object, 172) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(marker)),
           "byte 40, float 28, marker");
    expect(word(object, 12) == 16385u, "word 12 replaced");
    expect(object[155] == 1 && std::memcmp(object + 76, V, 12) == 0, "velocity copied");
    expect(flt(object, 164) == P[2] - (A[2] - P[2]) * 3.0f, "z derived from the object height");
    expect(half(object, 170) == 617 && word(object, 176) == 0xffffffffu && word(object, 180) == 0xffffffffu, "617 and all-ones slot");
    expect(half(object, 56) == static_cast<unsigned short>(0x1234 - 16384), "heading minus 16384");
    expect(word(object, 148) == 0xffffffffu && object[157] == 0xff, "word 148 and value low byte");
    expect(object[152] == 0 && object[153] == 0 && object[154] == 0 && object[156] == 0 && word(object, 160) == 0u &&
               half(object, 168) == 0, "cleared fields");
    expect(byte156_at_run_list == 0 && run_list_object == object, "byte 156 cleared before the run-list call");
    expect(object[158] == 0xcc && object[159] == 0xcc && object[184] == 0xcc && object[188] == 0xcc, "neighbouring bytes untouched");

    // Velocity null.
    order.clear();
    std::memset(object, 0xcc, sizeof object);
    __11FireBombletG6Point3sPcT1P6Point3i(object, P, -20000, marker, A, nullptr, 3);
    expect(object[155] == 0 && flt(object, 84) == 16.0f, "null velocity flag and 16.0");
    const float k1 = f(0x3e88b49eu), k2 = f(0x3c888888u);
    expect(flt(object, 76) == (P[0] - A[0]) * k1 * k2 && flt(object, 80) == (P[1] - A[1]) * k1 * k2, "velocity derived from P and A");
    expect(flt(object, 164) == A[2], "z is the anchor height");
    expect(half(object, 56) == static_cast<unsigned short>(-20000 - 16384), "negative heading wraps through 16 bits");
    expect(object[157] == 3, "value stored");
    expect(order.size() == 3, "same three calls");
    if (bad) return 1;
    std::puts("fire bomblet constructor contract passed");
    return 0;
}
