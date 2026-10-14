// Host check only; not a build input.
#include "set_space.cpp"
#include <cstring>
static const void* got_src; static void* got_dst;
extern "C" void sceVu0CopyMatrix(void* d, const void* s) { got_dst = d; got_src = s; }
int main() {
    alignas(16) unsigned char e[256]; alignas(16) unsigned char m[64];
    SetSpace__17VIParticleEmitterRC10VIMatrix44(e, m);
    return !(got_dst == e + 96 && got_src == m);
}
