#include "ctor.cpp"
#include <string.h>

static void* calls[4];
static int n;
extern "C" void __7VILList(void* l) { calls[n++] = l; }
extern "C" void __t6VIPool1ZP20VIParticleDefinition(void* p) { calls[n++] = p; }

int main() {
    alignas(8) unsigned char object[128];
    memset(object, 0xa5, sizeof object);
    unsigned char before[128];
    memcpy(before, object, sizeof object);
    __16VIParticleSystem(object);
    if (n != 3 || calls[0] != object + 52 || calls[1] != object + 64 || calls[2] != object + 84) return 1;
    unsigned char expect[128];
    memcpy(expect, before, sizeof expect);
    memset(expect + 0, 0, 4);
    memset(expect + 8, 0, 8);
    memset(expect + 76, 0, 8);   // words 76 and 80
    if (memcmp(expect, object, sizeof object) != 0) return 2;
    return 0;
}
