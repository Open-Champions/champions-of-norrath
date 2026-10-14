#include "ctor.cpp"
#include <string.h>
static unsigned gal, gsz; static void* gret; static void* garea; static int ga, gb, gc; static int order;
static int memorder, initorder;
extern "C" void* __wrap_memalign(unsigned a, unsigned s) { gal = a; gsz = s; memorder = ++order; return gret; }
extern "C" int sceLibnetInitialize(void* area, int a, int b, int c) { garea = area; ga = a; gb = b; gc = c; initorder = ++order; return 0; }
int main() {
    alignas(8) unsigned char obj[128]; memset(obj, 0x77, sizeof obj);
    int token; gret = &token;
    __11VIUDPSocketi(obj, 4242);
    if (*reinterpret_cast<int*>(obj) != 4242) return 1;
    if (*reinterpret_cast<int*>(obj + 68) != -1) return 2;
    if (*reinterpret_cast<void**>(obj + 4) != gret) return 3;
    if (gal != 64 || gsz != 20480) return 4;
    if (garea != obj + 8 || ga != 2048 || gb != 8192 || gc != 32) return 5;
    if (!(memorder == 1 && initorder == 2)) return 6;
    for (int i = 0; i < 128; ++i) if (i >= 12 && i < 68 && obj[i] != 0x77) return 7;
    for (int i = 72; i < 128; ++i) if (obj[i] != 0x77) return 8;
    return 0;
}
