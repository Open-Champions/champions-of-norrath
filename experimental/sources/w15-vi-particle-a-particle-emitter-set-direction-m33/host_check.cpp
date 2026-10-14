// Host check only; not a build input.
#include "set_direction.cpp"
static const void* got_src; static void* got_dst;
extern "C" void* __as__10VIMatrix33RC10VIMatrix33(void* d, const void* s) { got_dst = d; got_src = s; return d; }
int main() {
    alignas(16) unsigned char e[256]; alignas(16) unsigned char m[48];
    SetDirection__17VIParticleEmitterRC10VIMatrix33(e, m);
    return !(got_dst == e + 40 && got_src == m);
}
