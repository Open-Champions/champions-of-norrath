#define NORRATH_HOST_ARENA 1
#include "particle_definition.cpp"

static unsigned char arena[0x400];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }

int main() {
    alignas(4) unsigned char system[128] = {};
    *reinterpret_cast<unsigned*>(system + 92) = 0x100;      // records at arena offset 0x100
    for (int i = 0; i < 5; i++) {
        unsigned* rec = reinterpret_cast<unsigned*>(arena + 0x100 + 12 * i);
        rec[0] = 0x1000u + i; rec[1] = 0xdead; rec[2] = 0xbeef;
    }
    for (int i = 0; i < 5; i++)
        if (ParticleDefinition__16VIParticleSystemi(system, i) != 0x1000u + i) return 1 + i;
    return 0;
}
