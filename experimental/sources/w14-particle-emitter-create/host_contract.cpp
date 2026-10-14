#define NORRATH_HOST_ARENA 1
#include "emitter_create.cpp"
#include <string>
#include <string.h>

static unsigned char arena[0x2000];
static std::string tr;
static unsigned char* ebase;
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }
extern "C" unsigned norrath_host_word(const void* p) { return (unsigned)((const unsigned char*)p - arena); }
static std::string off(const void* p) { return std::to_string((const unsigned char*)p - arena); }
// each hook records what the emitter held when it ran
static std::string snap() {
    return "[" + std::to_string(*(unsigned*)(ebase + 80)) + "," + std::to_string(*(unsigned*)(ebase + 84)) + "," +
           std::to_string(*(unsigned*)(ebase + 912)) + "," + std::to_string(*(unsigned*)(ebase + 76)) + "," +
           std::to_string(*(unsigned*)(ebase + 980)) + "," + std::to_string(*(unsigned*)(ebase + 188 + 8)) + "," +
           std::to_string(*(unsigned*)(ebase + 16)) + "]";
}
extern "C" void Destroy__17VIParticleEmitter(void* e) { tr += "D" + off(e) + snap() + ";"; }
extern "C" void Identity__10VIMatrix33(void* m) { tr += "I3_" + off(m) + snap() + ";"; }
extern "C" void Identity__10VIMatrix44(void* m) { tr += "I4_" + off(m) + snap() + ";"; }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) { tr += "A" + off(l) + "," + off(n) + snap() + ";"; }
extern "C" void RecomputeStaticBBox__17VIParticleEmitter(void* e) { tr += "R" + off(e) + snap() + ";"; }

int main() {
    ebase = arena + 0x100;
    unsigned char* system = arena + 0x1000;
    unsigned char* def = arena + 0x1400;
    for (int i = 0; i < 0x2000; i++) arena[i] = (unsigned char)(i * 7 + 3);
    *(unsigned*)(ebase + 80) = 0x55;
    int r = Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(ebase, system, def);
    if (r != 0) return 1;
    // snapshot strings show the memory at each call: 80/84 set, 912/76 zero, then location zero before AddTail
    unsigned sw = (unsigned)(system - arena), dw = (unsigned)(def - arena);
    // order of the five calls
    size_t pD = tr.find("D256"), pI3 = tr.find("I3_296"), pI4 = tr.find("I4_352"), pA = tr.find("A" + std::to_string(sw + 52) + ",256"), pR = tr.find("R256");
    if (pD == std::string::npos || pI3 == std::string::npos || pI4 == std::string::npos || pA == std::string::npos || pR == std::string::npos) return 2;
    if (!(pD < pI3 && pI3 < pI4 && pI4 < pA && pA < pR)) return 3;
    // at Identity33 the pointers are stored; at AddTail the location is already zero
    std::string at_i3 = tr.substr(pI3, tr.find(';', pI3) - pI3);
    if (at_i3.find("[" + std::to_string(sw) + "," + std::to_string(dw) + ",0,0,") == std::string::npos) return 4;
    std::string at_a = tr.substr(pA, tr.find(';', pA) - pA);
    if (at_a.find(",0,0,0]") == std::string::npos && at_a.find(",0]") == std::string::npos) return 5;
    // final memory
    if (*(unsigned*)(ebase + 80) != sw || *(unsigned*)(ebase + 84) != dw) return 6;
    if (memcmp(ebase + 188, def, 724) != 0) return 7;
    for (int i : {76, 172, 912, 980, 984}) if (*(unsigned*)(ebase + i) != 0) return 8;
    for (int i = 0; i < 3; i++) {
        if (((float*)(ebase + 16))[i] != 0 || ((float*)(ebase + 28))[i] != 0) return 9;
        if (((float*)(ebase + 940))[i] != 0 || ((float*)(ebase + 952))[i] != 0) return 10;
    }
    // bytes the function must not touch
    unsigned char ref[0x2000];
    for (int i = 0; i < 0x2000; i++) ref[i] = (unsigned char)(i * 7 + 3);
    for (int i = 0; i < 0x2000; i++) {
        int rel = i - 0x100;
        bool written = (rel >= 76 && rel < 88) || (rel >= 16 && rel < 40) || (rel >= 172 && rel < 176) ||
                       (rel >= 188 && rel < 188 + 724) || (rel >= 912 && rel < 916) ||
                       (rel >= 940 && rel < 964) || (rel >= 980 && rel < 988) ||
                       (rel >= 40 && rel < 76) || (rel >= 96 && rel < 160);   // identity matrices (mocked: untouched)
        if (!written && arena[i] != ref[i]) return 11;
    }
    return 0;
}
