// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "share.cpp"
#include <string.h>
static unsigned char arena[0x2000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }
int main() {
    memset(arena, 0x11, sizeof arena);
    unsigned char* sys = arena;
    *(unsigned*)(sys + 92) = 0x100;             // table at arena+0x100
    *(unsigned*)(arena + 0x100 + 12 * 3) = 0x400;   // record 3 -> definition at 0x400
    *(unsigned*)(arena + 0x400 + 768) = 5;
    unsigned char copy[sizeof arena];
    memcpy(copy, arena, sizeof arena);
    if (ShareParticleDefinition__16VIParticleSystemi(sys, 3) != 0) return 1;
    if (*(unsigned*)(arena + 0x400 + 768) != 6) return 2;
    *(unsigned*)(copy + 0x400 + 768) = 6;
    if (memcmp(copy, arena, sizeof arena)) return 3;
    *(unsigned*)(arena + 0x100) = 0x800;        // record 0
    ShareParticleDefinition__16VIParticleSystemi(sys, 0);
    ShareParticleDefinition__16VIParticleSystemi(sys, 0);
    if (*(unsigned*)(arena + 0x800 + 768) != 0x11111111u + 2) return 4;
    return 0;
}
