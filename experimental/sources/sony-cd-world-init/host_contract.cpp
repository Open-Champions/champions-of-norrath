#include <cstdint>
#define NORRATH_HOST_CONTRACT 1
#include "world_init.cpp"

extern "C" int megatile_ramkmax = 0;
extern "C" int nmParticleEmitterRecs = 7;
extern "C" short texmapstart = 0;
extern "C" short texmapend = 0;
extern "C" unsigned char texmap[16] = {};
extern "C" char engineCurrentLevelName[] = "cave";
extern "C" unsigned char DynamicLightArray[16 * 68 + 8] = {};
extern "C" unsigned char ParticleEmitterArray[152] = {};
static int lights = 0;
static int loads = 0;
static int updates = 0;
static int early = 0;
static char loaded_name[32];
extern "C" int puts(const char* text);
static int same_text(const char* left, const char* right) {
    while (*left != 0 && *left == *right) {
        left += 1;
        right += 1;
    }
    return *left == *right;
}
extern "C" int sprintf(char* dest, const char* format, const char* name) {
    if (!same_text(format, "%s.tex")) return -1;
    while (*name != 0) *dest++ = *name++;
    const char* suffix = ".tex";
    while (*suffix != 0) *dest++ = *suffix++;
    *dest = 0;
    return 0;
}
extern "C" int cdLoad__FPcPvii(char* name, void* destination, int bytes, int argument) {
    loads += 1;
    for (int index = 0; index < 31 && name[index] != 0; ++index) loaded_name[index] = name[index];
    if (bytes != 20480 || argument != 0 || (reinterpret_cast<std::uintptr_t>(destination) & 63u) != 0u) return -1;
    auto* bytes_out = static_cast<unsigned char*>(destination);
    bytes_out[4] = 0xfe;
    bytes_out[5] = 0xff;
    bytes_out[6] = 0xff;
    bytes_out[7] = 0xff;
    return 0;
}
extern "C" void lightDeactivate__FP5Light(void* light) {
    if (light == DynamicLightArray + 8 + lights * 68) lights += 1;
}
extern "C" int __eq__19ParticleEmitterDataRC19ParticleEmitterData(void*, void*) { return 0; }
extern "C" void P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(
    void* definition, int a, int, int, int, int, int, int, float x, float, float, int, int, int, int, void (*)(void*)) {
    updates += 1;
    if (definition != ParticleEmitterArray + 72 || a != 3 || x != 8.f) updates = -100;
}
extern "C" void textureInitTexture__FP8_texture(void*) {}
extern "C" void megatileInit__FP12MegatileRootPv(void*, void*) {}
extern "C" void _engineCheckStack__FPCci(const char*, int) {}
extern "C" int scePrintf(const char*) { return 0; }
extern "C" void megatileGenerate__FP12_worldHeader(void*) {}
extern "C" void earlyInit__3mapP12_worldHeader(void*) { early += 1; }
static unsigned word_at(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static void put_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    unsigned char header[256] = {};
    put_word(header, 1);
    put_word(header + 4, 200);
    header[200] = 0x67;
    header[201] = 0x45;
    put_word(header + 24, 80);
    put_word(header + 16, 1);
    put_word(header + 20, 1);
    put_word(header + 80, 5);
    put_word(header + 92, 0xffffffffu);
    put_word(header + 120, 9);
    put_word(header + 36, 140);
    put_word(header + 140, 160);
    put_word(header + 176, 0x4000u);
    header[160] = 3;
    header[168] = 8;
    if (worldInitWorld__FP12_worldHeader(header) != 0) return 1;
    const unsigned base = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(header));
    if (word_at(header + 24) != base + 80u || word_at(header + 80) != base + 14u) return 2;
    if (word_at(header + 36) != base + 140u) return 3;
    if (!same_text(loaded_name, "cave.tex") || loads != 1) return 4;
    if (megatile_ramkmax != 15360 || lights != 16) return 5;
    if (updates != 1 || nmParticleEmitterRecs != 1 || ParticleEmitterArray[0] != 3) return 6;
    if (word_at(header + 104) != 0x44700000u || early != 1) return 7;
    if (word_at(header + 4) != base + 202u) return 8;
    unsigned char map[160] = {};
    put_word(map + 88, 4);
    put_word(map + 92, 7);
    put_word(map + 100, 8);
    put_word(map + 116, 1);
    if (worldInitWorld__FP12_worldHeader(map) != 0) return 9;
    if (texmapstart != 1 || texmapend != 1 || texmap[0] != 1 || texmap[1] != 0) return 10;
    unsigned char generated[160] = {};
    put_word(generated + 112, 40);
    put_word(generated + 120, 15);
    if (worldInitWorld__FP12_worldHeader(generated) != 15) return 11;
    unsigned char skipped[160] = {};
    put_word(skipped, 1);
    put_word(skipped + 36, 140);
    put_word(skipped + 176, 0x80u);
    const int updates_before = updates;
    worldInitWorld__FP12_worldHeader(skipped);
    if (updates != updates_before) return 12;
    puts("world init contract passed");
    return 0;
}
