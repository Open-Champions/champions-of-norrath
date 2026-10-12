// Host contract for __19ParticleMorphEffectP8CreatureT1. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" {
unsigned frameNm = 0;
unsigned char vt_19ParticleMorphEffect[16] __asm__("_vt$19ParticleMorphEffect");
unsigned __19ParticleMorphEffectP8CreatureT1(void* object, void* first, void* second);
}
static int order = 0, construct_at = 0, run_at = 0;
static float point_seen[3] = {1, 1, 1};
static void* resource_map_seen = reinterpret_cast<void*>(1);
static char** arguments_seen = reinterpret_cast<char**>(1);
static unsigned char object_at_construct[128];
static void* run_list_object = nullptr;
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments) {
    construct_at = ++order;
    std::memcpy(point_seen, point, 12);
    resource_map_seen = resource_map;
    arguments_seen = arguments;
    // The constructor leaves a flags word for the caller to extend.
    const unsigned flags = 0x00010000u;
    std::memcpy(static_cast<unsigned char*>(object) + 12, &flags, 4);
    return 0;
}
extern "C" unsigned norrath_actor_add_run_list(void* object) {
    run_at = ++order;
    run_list_object = object;
    std::memcpy(object_at_construct, object, sizeof object_at_construct);
    return 0xabcdu;
}
static unsigned word(const unsigned char* b, unsigned o) { unsigned v; std::memcpy(&v, b + o, 4); return v; }
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); }

int main() {
    alignas(8) unsigned char object[128];
    std::memset(object, 0xcc, sizeof object);
    int a, b;
    frameNm = 1000;
    const unsigned result = __19ParticleMorphEffectP8CreatureT1(object, &a, &b);
    int bad = 0;
    auto expect = [&](bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); bad = 1; } };
    expect(construct_at == 1 && run_at == 2, "constructor first, run list last");
    expect(point_seen[0] == 0.0f && point_seen[1] == 0.0f && point_seen[2] == 0.0f, "zero point");
    expect(resource_map_seen == nullptr && arguments_seen == nullptr, "null resource map and arguments");
    expect(run_list_object == object && result == 0xabcdu, "run list result returned");
    expect(word(object, 12) == (0x00010000u | 0x421fu), "flags read after the constructor and ORed");
    expect(word(object, 72) == low(vt_19ParticleMorphEffect), "vtable");
    expect(word(object, 76) == 1150u && word(object, 80) == 1000u, "frame words");
    expect(word(object, 84) == low(&a) && word(object, 88) == low(&b), "creature pointers in order");
    expect(object[11] == 0xcc && object[92] == 0xcc && object[68] == 0xcc, "other bytes untouched");
    // The stores are done before the run-list call.
    expect(word(object_at_construct, 80) == 1000u, "stores precede the run-list call");
    if (bad) return 1;
    std::printf("morph-effect-ctor contract passed\n");
    return 0;
}
