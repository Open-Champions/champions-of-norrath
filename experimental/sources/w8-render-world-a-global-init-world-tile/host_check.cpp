#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
extern "C" {
unsigned char ParticleEmitterArray[152 * 16];
unsigned char DynamicLightArray[68 * 16];
unsigned char routeQueue[48 * 128];
unsigned char droplet[80];
unsigned char rainSplashParticle[80];
unsigned char worldAnimation[48 * 64];
unsigned char ParticleEmitterTestParticle[80];
}
struct Call {
    std::string name;
    const void* self;
    int ints[8];
    float floats[3];
};
static std::vector<Call> calls;
extern "C" void __11ParticleDef(void* self) { calls.push_back({"basic", self, {}, {}}); }
extern "C" void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h, float x, float y, float z, int i, int j, int k, void (*callback)(void*)) {
    Call call{"full", self, {a, b, c, d, e, f, g, h}, {x, y, z}};
    calls.push_back(call);
    calls.push_back({"full-tail", reinterpret_cast<const void*>(callback), {i, j, k}, {}});
}



#include "global_init_world_tile.cpp"
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    std::memset(ParticleEmitterArray, 0xEE, sizeof ParticleEmitterArray);
    std::memset(DynamicLightArray, 0xEE, sizeof DynamicLightArray);
    std::memset(routeQueue, 0xEE, sizeof routeQueue);
    std::memset(worldAnimation, 0xEE, sizeof worldAnimation);
    _GLOBAL_$I$worldTileEntityLightScale();
    expect(calls.size() == 16 + 6, "16 basic constructors and three full constructors (two records each)");
    for (int i = 0; i < 16; ++i) expect(calls[i].name == "basic" && calls[i].self == ParticleEmitterArray + 152 * i + 72, "basic constructor on the +72 member");
    expect(calls[16].name == "full" && calls[16].self == droplet, "first full constructor on droplet");
    expect(calls[16].ints[0] == 70 && calls[16].ints[1] == 70 && calls[16].ints[2] == 150 && calls[16].ints[3] == 50 &&
           calls[16].ints[4] == 60 && calls[16].ints[5] == 60 && calls[16].ints[6] == 170 && calls[16].ints[7] == 230, "droplet integers");
    expect(calls[16].floats[0] == __builtin_bit_cast(float, 0x412ae147u) && calls[16].floats[2] == __builtin_bit_cast(float, 0xc2701eb8u), "droplet floats");
    expect(calls[17].ints[0] == 3 && calls[17].ints[1] == 2 && calls[17].ints[2] == 0 && calls[17].self == nullptr, "droplet tail arguments");
    expect(calls[18].name == "full" && calls[18].self == rainSplashParticle && calls[18].ints[0] == 50 && calls[18].ints[7] == 0 &&
           calls[18].floats[0] == 1.0f && calls[18].floats[1] == 10.0f && calls[18].floats[2] == 0.0f, "rain splash");
    expect(calls[19].ints[0] == 1 && calls[19].ints[1] == 1 && calls[19].ints[2] == 7, "rain splash tail");
    expect(calls[20].name == "full" && calls[20].self == ParticleEmitterTestParticle && calls[20].ints[0] == 130 && calls[20].ints[3] == 0 &&
           calls[20].ints[7] == 205 && calls[20].floats[0] == 5.0f && calls[20].floats[1] == 5.0f && calls[20].floats[2] == 1.0f, "test particle");
    expect(calls[21].ints[0] == 3 && calls[21].ints[1] == 1 && calls[21].ints[2] == 4, "test particle tail");
    for (int i = 0; i < 16; ++i) expect(DynamicLightArray[68 * i + 16] == 0 && DynamicLightArray[68 * i + 17] == 0 && DynamicLightArray[68 * i + 18] == 0xEE && DynamicLightArray[68 * i + 15] == 0xEE, "light flags cleared");
    for (int i = 0; i < 128; ++i) expect(*reinterpret_cast<unsigned*>(routeQueue + 48 * i + 40) == 0 && routeQueue[48 * i + 39] == 0xEE && routeQueue[48 * i + 44] == 0xEE, "route queue word cleared");
    for (int i = 0; i < 64; ++i) expect(worldAnimation[48 * i + 5] == 1 && worldAnimation[48 * i + 4] == 0xEE && worldAnimation[48 * i + 6] == 0xEE, "animation byte set");

    return failures;
}
