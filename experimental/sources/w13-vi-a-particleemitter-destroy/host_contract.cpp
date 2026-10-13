#define NORRATH_HOST_ARENA 1
#include "destroy.cpp"
#include <string.h>
#include <string>

static unsigned char arena[0x1000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }
static std::string tr; static int nodes_left;
static unsigned char* ebase;
extern "C" void Remove__7VILNode(void* n) { tr += "R" + std::to_string((unsigned char*)n - ebase) + ";"; }
extern "C" void* RemHead__7VILList(void* l) { tr += "H" + std::to_string((unsigned char*)l - ebase) + ";"; return nodes_left-- > 0 ? (void*)(arena + 0x800 + 16 * nodes_left) : 0; }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) { tr += "A" + std::to_string((unsigned char*)l - arena) + "," + std::to_string((unsigned char*)n - arena) + ";"; }
extern "C" void Identity__10VIMatrix44(void* m) { tr += "I" + std::to_string((unsigned char*)m - ebase) + ";"; }
extern "C" void UnlinkAllMotifBlenders__17VIParticleEmitter(void* e) { tr += "U" + std::to_string((unsigned char*)e - ebase) + ";"; }

int main() {
    ebase = arena + 0x100;
    memset(arena, 0x33, sizeof arena);
    *(unsigned*)(ebase + 80) = 0;
    Destroy__17VIParticleEmitter(ebase);
    if (tr != "U0;") return 1;
    tr.clear(); nodes_left = 2;
    *(unsigned*)(ebase + 80) = 0x40;                  // owner at arena offset 0x40
    Destroy__17VIParticleEmitter(ebase);
    if (tr != "R0;H160;A128,2064;H160;A128,2048;H160;I96;U0;") return 2;
    if (*(unsigned*)(ebase + 80) != 0 || *(unsigned*)(ebase + 84) != 0 || *(unsigned*)(ebase + 172) != 0) return 3;
    if (ebase[79] != 0x33 || ebase[88] != 0x33 || ebase[176] != 0x33 || ebase[171] != 0x33) return 4;
    return 0;
}
