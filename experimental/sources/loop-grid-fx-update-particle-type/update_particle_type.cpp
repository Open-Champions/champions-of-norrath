// Observed P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v at
// 0x180ac0, extent 736. The 80 bytes of the definition are copied aside, then
// setParticleDef (0x180820, same argument list) rewrites the definition. If
// the descriptor slot word at +4 is -1 nothing more happens. If the 80 bytes
// are unchanged (memcmp is zero) nothing more happens. Otherwise bytes 16 to
// 79 of the definition are copied to the 64-byte record descList[slot] at
// 0x780300 + slot * 64. The caller's arguments are forwarded unchanged: eight
// integers, three floats, three integers, then a callback pointer.
extern "C" {
extern unsigned char descList[];
void setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(unsigned char* definition, int a, int b, int c, int d, int e, int f, int g, int h,
                                                         float x, float y, float z, int i, int j, int k, void (*callback)(void*));
int norrath_memory_compare(const void* left, const void* right, __SIZE_TYPE__ count);
}

namespace {
// The definition may be only word aligned. Doubleword copies of a packed type
// use the left and right load and store forms, as the original does.
struct Doubleword { unsigned long long value; } __attribute__((packed, may_alias));
}

extern "C" void P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(unsigned char* definition, int a, int b, int c, int d, int e, int f,
                                                                           int g, int h, float x, float y, float z, int i, int j, int k,
                                                                           void (*callback)(void*)) {
    alignas(8) unsigned long long before[10];
    const Doubleword* source = reinterpret_cast<const Doubleword*>(definition);
    for (int word = 0; word < 10; ++word) before[word] = source[word].value;
    setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(definition, a, b, c, d, e, f, g, h, x, y, z, i, j, k, callback);
    const unsigned slot = static_cast<unsigned>(definition[4]) | (static_cast<unsigned>(definition[5]) << 8) |
                          (static_cast<unsigned>(definition[6]) << 16) | (static_cast<unsigned>(definition[7]) << 24);
    if (slot == 0xffffffffu) return;
    if (norrath_memory_compare(before, definition, 80u) == 0) return;
    Doubleword* destination = reinterpret_cast<Doubleword*>(descList + slot * 64u);
    for (int word = 0; word < 8; ++word) destination[word].value = source[2 + word].value;
}
