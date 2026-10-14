// Host check only; not a build input.
#include <cstring>
#include <string>
#define memset norrath_host_memset
#include "alloc_particles.cpp"
#undef memset
static std::string trace;
static float boxSize[3];
static void* mallocResult;
static unsigned mallocSize;
static unsigned char block[4096];
extern "C" void Size__C6VIBBox(float* r, const void*) { trace += "S;"; std::memcpy(r, boxSize, sizeof boxSize); }
extern "C" void FreeParticles__14VIEffectVolume(void*) { trace += "F;"; }
extern "C" void* __wrap_malloc(unsigned n) { trace += "M;"; mallocSize = n; return mallocResult; }
extern "C" void* norrath_host_memset(void* d, int v, unsigned n) { trace += "Z" + std::to_string(n) + ";"; unsigned char* p = static_cast<unsigned char*>(d); for (unsigned i = 0; i < n; ++i) p[i] = static_cast<unsigned char>(v); return d; }
static unsigned& word(unsigned char* o, int off) { return *reinterpret_cast<unsigned*>(o + off); }
int main() {
    alignas(8) static unsigned char o[128];
    boxSize[0] = 10.0f; boxSize[1] = 5.0f; boxSize[2] = 1.0f;
    float density = 0.55f;
    std::memcpy(o + 44, &density, 4);
    mallocResult = block;
    std::memset(block, 0xaa, sizeof block);
    word(o, 36) = 1;
    if (AllocParticles__14VIEffectVolume(o) != 0) return 1;
    // 10 * 5 = 50 * 0.4 (just under 20) * 0.55 = just under 11: 10 records of 36 bytes
    if (word(o, 84) != 10 || mallocSize != 360 || trace != "S;F;M;Z360;" || word(o, 32) != 1 || block[0] != 0 || block[359] != 0 || block[360] != 0xaa) return 2;
    trace.clear(); word(o, 36) = 2; word(o, 32) = 0;
    AllocParticles__14VIEffectVolume(o);
    if (word(o, 84) != 2 || mallocSize != 64) return 3;           // 10 * 0.5 * 0.5 = 2.5 -> 2 records of 32
    word(o, 36) = 3; AllocParticles__14VIEffectVolume(o);
    if (word(o, 84) != 110 || mallocSize != 3960) return 4;       // 10 * 20 * 0.55
    word(o, 36) = 4; density = 0.01f; std::memcpy(o + 44, &density, 4);
    AllocParticles__14VIEffectVolume(o);
    if (word(o, 84) != 2 || mallocSize != 120) return 5;          // 2.0 -> 2 records of 60
    density = 0.0f; std::memcpy(o + 44, &density, 4);
    AllocParticles__14VIEffectVolume(o);
    if (word(o, 84) != 1 || mallocSize != 60) return 6;           // clamped to one record
    trace.clear(); word(o, 32) = 0; mallocResult = nullptr;
    if (AllocParticles__14VIEffectVolume(o) != -1 || word(o, 88) != 0 || word(o, 32) != 0 || trace != "S;F;M;") return 7;
    trace.clear(); word(o, 36) = 0;
    if (AllocParticles__14VIEffectVolume(o) != 0 || trace != "S;F;F;" || word(o, 32) != 0) return 8;
    trace.clear(); word(o, 36) = 9;
    if (AllocParticles__14VIEffectVolume(o) != 0 || trace != "S;F;" || word(o, 32) != 1) return 9;
    return 0;
}
