#define NORRATH_HOST_ARENA 1
#include "emitter_update.cpp"
#include <string>
#include <string.h>

// Scripted stubs: rand returns 5 every time, so every (r1 - r2) term is zero.
static unsigned char arena[0x4000];
static unsigned char* E = arena + 0x100;
static std::string tr;
static int avail, texture, taken;
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }
extern "C" void BlendMotifs__17VIParticleEmitter(void*) { tr += "B;"; }
extern "C" int rand() { return 5; }
extern "C" float Distance__C7VIVect3RC7VIVect3(const void*, const void*) { tr += "Dist;"; return 4.0f; }
extern "C" int TextureConfiguration__C22VIParticleDefinitionEx(const void*) { tr += "T;"; return texture; }
extern "C" void __ml__FRC7VIVect3RC10VIMatrix33(void* out, const void* v, const void*) { tr += "ml;"; memcpy(out, v, 12); }
extern "C" void RotateHPR__10VIMatrix44RC7VIVect3(void*, const void*) { tr += "rot;"; }
extern "C" void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void*, const void* in, void* out) { tr += "mul;"; memcpy(out, in, 12); }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) { tr += "A" + std::to_string((unsigned char*)l - E) + ";"; (void)n; }
extern "C" void* RemHead__7VILList(void*) {
    tr += "H;";
    return taken < avail ? arena + 0x2000 + 128 * taken++ : 0;
}
extern "C" void _$_10VIMatrix44(void* m, int f) { tr += "M" + std::to_string(f) + ";"; for (int i = 0; i < 64; i++) if (((unsigned char*)m)[i]) tr += "!"; }


static void reset() {
    for (int i = 0; i < 0x4000; i++) arena[i] = 0;
    *(unsigned*)(E + 80) = 0x1000;
    *(unsigned*)(E + 84) = 0x1100;
    *(float*)(arena + 0x1000 + 16) = 0.5f;          // dt
    *(float*)(E + 192) = 4.0f;                       // birth rate -> spawn 2.0
    *(float*)(E + 200) = 2.0f;                       // lifespan
    *(float*)(E + 188) = 0.75f;                      // friction copy
    *(float*)(E + 228) = 8.0f; *(float*)(E + 236) = 2.0f;
    *(float*)(E + 208) = 3.0f;                       // velocity
    *(float*)(E + 848 + 8) = 1.0f;                   // nozzle axis (0,0,1)
    *(float*)(E + 16) = 10.0f; *(float*)(E + 28) = 2.0f;
    tr.clear(); texture = 1; avail = 5; taken = 0;
}

int main() {
    reset();
    *(unsigned*)(E + 80) = 0;
    if (Update__17VIParticleEmitter(E) != -1 || !tr.empty()) return 1;
    reset();
    *(unsigned*)(E + 84) = 0;
    if (Update__17VIParticleEmitter(E) != -1 || !tr.empty()) return 2;

    reset();
    if (Update__17VIParticleEmitter(E) != 0) return 3;
    // spawn = 4 * 0.5 = 2.0 -> two particles; remainder 0; both linked to emitter + 160
    if (tr != "B;T;H;A160;ml;ml;H;A160;ml;ml;M2;") return 4;
    // BlendMotifs ran first; each particle took the two RemHead nodes; temp matrix zero at destruction
    if (*(unsigned*)(E + 172) != 2) return 5;
    if (*(float*)(E + 76) != 0.0f) return 6;
    unsigned char* n0 = arena + 0x2000;
    unsigned char* n1 = arena + 0x2000 + 128;
    if (*(float*)(n0 + 20) != 0.5f || *(float*)(n0 + 24) != 0.75f) return 7;       // 1/lifespan, friction
    if (*(float*)(n0 + 88) != 4.0f) return 8;                                     // start size 8 * 0.5
    if (*(float*)(n0 + 92) != (1.0f - 4.0f) * 0.5f) return 9;                     // (2 * 0.5 - 4) * 0.5
    if (*(unsigned*)(n0 + 100) != 0) return 10;                                   // base 0 for texture 1
    // direction: axis (0,0,1) * speed 3
    if (*(float*)(n0 + 44) != 0.0f || *(float*)(n0 + 48) != 0.0f || *(float*)(n0 + 52) != 3.0f) return 11;
    // positions: pos starts at last location 2.0 and steps by (10 - 2) / 2 = 4
    if (*(float*)(n0 + 32) != 2.0f || *(float*)(n1 + 32) != 6.0f) return 12;
    // second particle has its own frame number only when base != 0
    reset(); texture = 3; avail = 1;
    Update__17VIParticleEmitter(E);
    if (*(unsigned*)(arena + 0x2000 + 100) != 1 + 5 % 3) return 13;
    // inherit velocity and delta spawn paths
    reset(); *(float*)(E + 788) = 0.5f; *(float*)(E + 792) = 1.0f; avail = 1;
    *(float*)(E + 76) = 0.25f;
    Update__17VIParticleEmitter(E);
    // spawn = 2.0 + 4.0 (distance) * 1.0 + 0.25 = 6.25 -> 6 particles wanted, 1 available: stops at null
    if (tr.find("Dist;") == std::string::npos) return 14;
    if (*(float*)(E + 76) != 0.25f) return 15;
    // inherit: (10 - 2) * (1 / 0.5) * 0.5 = 8 added to velocity x
    if (*(float*)(arena + 0x2000 + 44) != 8.0f) return 16;
    return 0;
}
