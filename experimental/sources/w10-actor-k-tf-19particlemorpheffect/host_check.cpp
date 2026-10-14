// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10GameObject[12];
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "__tf10GameObject;"; return nullptr; }
extern "C" void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + (t == __ti19ParticleMorphEffect ? "node" : "?") + "," + name + "," + (b == __ti10GameObject ? "base" : "?") + ");";
    static_cast<unsigned char*>(t)[0] = 1;  // the real call stores a name pointer
}
int main() {
    int bad = 0;
    std::memset(__ti19ParticleMorphEffect, 0, 12);
    if (__tf19ParticleMorphEffect() != __ti19ParticleMorphEffect) bad = 1;
    if (calls != "__tf10GameObject;si(node,19ParticleMorphEffect,base);") bad = 1;
    calls.clear();
    if (__tf19ParticleMorphEffect() != __ti19ParticleMorphEffect || !calls.empty()) bad = 1;
    for (int byte = 0; byte < 4; ++byte) {
        std::memset(__ti19ParticleMorphEffect, 0, 12); __ti19ParticleMorphEffect[byte] = 1; calls.clear();
        if (__tf19ParticleMorphEffect() != __ti19ParticleMorphEffect || !calls.empty()) bad = 1;
    }
    std::memset(__ti19ParticleMorphEffect, 0, 12); __ti19ParticleMorphEffect[4] = 9; __ti19ParticleMorphEffect[11] = 4; calls.clear();
    __tf19ParticleMorphEffect();
    if (calls.empty()) bad = 1;
    if (bad) std::puts("fail");
    return bad;
}
