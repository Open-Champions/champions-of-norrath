// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "free_particles.cpp"
static int calls;
static void* freed;
extern "C" void __wrap_free(void* p) { ++calls; freed = p; }
int main() {
    alignas(8) static unsigned char o[128];
    unsigned* w = reinterpret_cast<unsigned*>(o);
    w[22] = arena_base + 0x40;
    FreeParticles__14VIEffectVolume(o);
    if (calls != 1 || freed != arena + 0x40 || w[22] != 0) return 1;
    FreeParticles__14VIEffectVolume(o);
    return calls != 1 ? 2 : 0;
}
