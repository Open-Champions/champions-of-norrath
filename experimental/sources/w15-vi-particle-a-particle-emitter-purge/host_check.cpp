// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "purge.cpp"
#include <string.h>
#include <string>

static unsigned char arena[0x1000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }
static std::string tr; static int nodes_left;
static unsigned char* ebase;
extern "C" void* RemHead__7VILList(void* l) { tr += "H" + std::to_string((unsigned char*)l - ebase) + ";"; return nodes_left-- > 0 ? (void*)(arena + 0x800 + 16 * nodes_left) : 0; }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) { tr += "A" + std::to_string((unsigned char*)l - arena) + "," + std::to_string((unsigned char*)n - arena) + ";"; }

int main() {
    ebase = arena + 0x100;
    memset(arena, 0x33, sizeof arena);
    *(unsigned*)(ebase + 80) = 0x40;
    *(unsigned*)(ebase + 84) = 0x77;
    nodes_left = 0;
    Purge__17VIParticleEmitter(ebase);
    if (tr != "H160;") return 1;
    if (*(unsigned*)(ebase + 80) != 0x40 || *(unsigned*)(ebase + 84) != 0x77 || *(unsigned*)(ebase + 172) != 0) return 2;
    tr.clear(); nodes_left = 2;
    *(unsigned*)(ebase + 172) = 9;
    Purge__17VIParticleEmitter(ebase);
    if (tr != "H160;A128,2064;H160;A128,2048;H160;") return 3;
    if (*(unsigned*)(ebase + 80) != 0x40 || *(unsigned*)(ebase + 84) != 0x77 || *(unsigned*)(ebase + 172) != 0) return 4;
    if (ebase[171] != 0x33 || ebase[176] != 0x33 || ebase[79] != 0x33 || ebase[88] != 0x33) return 5;
    return 0;
}
