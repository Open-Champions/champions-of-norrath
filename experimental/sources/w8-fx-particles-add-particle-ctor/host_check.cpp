// Host contract for __11AddParticleG6Point3PPc. Not an EE plan input.
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" {
unsigned char vt_11AddParticle[16] __asm__("_vt$11AddParticle");
void __11AddParticleG6Point3PPc(void* object, const void* point, char** arguments);
}
static std::vector<const char*> order;
static unsigned char* current = nullptr;
static float point_seen[3];
static unsigned flags_at_particle_init = 0;
static unsigned char light_bytes[96];
static bool give_light = true;
static unsigned word(const unsigned char* b, unsigned o) { unsigned v; std::memcpy(&v, b + o, 4); return v; }
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments) {
    order.push_back("construct");
    std::memcpy(point_seen, point, 12);
    if (resource_map != nullptr || arguments != reinterpret_cast<char**>(0x1000)) std::fprintf(stderr, "fail: construct args\n");
    // The constructor leaves a flags word the body must extend.
    unsigned flags = 0x00100000u;
    std::memcpy(static_cast<unsigned char*>(object) + 12, &flags, 4);
    return 0;
}
extern "C" void __11ParticleDef(void* def) {
    order.push_back("def");
    if (def != current + 172) std::fprintf(stderr, "fail: def address\n");
}
extern "C" void particleInit__11AddParticlePPc(void* object, char**) {
    order.push_back("particleInit");
    flags_at_particle_init = word(static_cast<unsigned char*>(object), 12);
}
extern "C" void lightInit__11AddParticlePPc(void* object, char**) {
    order.push_back("lightInit");
    unsigned char* o = static_cast<unsigned char*>(object);
    const unsigned has = give_light ? 7u : 0u;
    std::memcpy(o + 84, &has, 4);
    const int radius = 10;
    std::memcpy(o + 324, &radius, 4);
    const short halves[4] = {-1, 2, 3, -4};
    for (int i = 0; i < 4; ++i) std::memcpy(o + 340 + 4 * i, &halves[i], 2);
}
extern "C" void norrath_light_add(void* light) {
    order.push_back("lightAdd");
    std::memcpy(light_bytes, light, sizeof light_bytes);
    if (light != current + 256) std::fprintf(stderr, "fail: light address\n");
}

int main() {
    alignas(16) unsigned char object[400];
    std::memset(object, 0xcc, sizeof object);
    current = object;
    const float point[3] = {1.0f, 2.0f, 3.0f};
    __11AddParticleG6Point3PPc(object, point, reinterpret_cast<char**>(0x1000));
    int bad = 0;
    auto expect = [&](bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); bad = 1; } };
    const char* want[] = {"construct", "def", "particleInit", "lightInit", "lightAdd"};
    expect(order.size() == 5, "five calls");
    for (size_t i = 0; i < order.size() && i < 5; ++i) expect(std::strcmp(order[i], want[i]) == 0, "call order");
    expect(point_seen[0] == 1.0f && point_seen[1] == 2.0f && point_seen[2] == 3.0f, "point passed through");
    expect(word(object, 72) == static_cast<unsigned>(reinterpret_cast<unsigned long>(vt_11AddParticle)), "vtable");
    expect(word(object, 76) == 0xffffffffu && word(object, 80) == 0u && word(object, 88) == 0u && word(object, 96) == 0u, "plain stores");
    expect(word(object, 252) == 0u && word(object, 320) == 1u, "+252 and +320");
    expect(flags_at_particle_init == (0x00100000u | 0x400fu) && word(object, 12) == flags_at_particle_init, "flags stored before particleInit");
    expect(std::memcmp(object + 304, point, 12) == 0, "position copy at +304");
    expect(object[264] == 0 && light_bytes[8] == 0 && light_bytes[9] == 0, "light flag bytes cleared before lightAdd");
    float radius, inverse_a, inverse_b;
    std::memcpy(&radius, object + 284, 4); std::memcpy(&inverse_a, object + 296, 4); std::memcpy(&inverse_b, object + 288, 4);
    expect(radius == 10.0f && std::fabs(inverse_a - 0.01f) < 1e-7f && inverse_a == inverse_b, "radius and inverse square");
    float radius2; std::memcpy(&radius2, object + 292, 4);
    expect(radius2 == 10.0f && word(object, 300) == 0u, "second radius and zero word");
    unsigned short got[8]; std::memcpy(got, object + 274, 2); std::memcpy(got + 1, object + 276, 2);
    std::memcpy(got + 2, object + 278, 2); std::memcpy(got + 3, object + 280, 2);
    std::memcpy(got + 4, object + 266, 2); std::memcpy(got + 5, object + 268, 2);
    std::memcpy(got + 6, object + 270, 2); std::memcpy(got + 7, object + 272, 2);
    const unsigned short want_h[4] = {0xffffu, 2u, 3u, 0xfffcu};
    for (int i = 0; i < 8; ++i) expect(got[i] == want_h[i % 4], "halfword copies");

    // No light: the function stops after lightInit without touching the light record.
    order.clear();
    give_light = false;
    std::memset(object, 0xcc, sizeof object);
    __11AddParticleG6Point3PPc(object, point, reinterpret_cast<char**>(0x1000));
    expect(order.size() == 4, "no lightAdd without a light");
    expect(object[304] == 0xcc && object[274] == 0xcc && object[266] == 0xcc, "light record untouched");
    expect(object[264] == 0 && object[265] == 0, "flag bytes are still cleared before the inits");
    if (bad) return 1;
    std::printf("add-particle-ctor contract passed\n");
    return 0;
}
