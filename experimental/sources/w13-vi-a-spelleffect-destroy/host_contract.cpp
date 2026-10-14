#include "destroy.cpp"
#include <string.h>

static void* seen[8]; static int n;
extern "C" void Destroy__16VISpellEventList(void* l) { seen[n++] = l; }

int main() {
    alignas(4) unsigned char e[64];
    memset(e, 0x11, sizeof e); *(int*)(e + 8) = 0;
    if (Destroy__13VISpellEffect(e) != -1 || n != 0) return 1;
    *(int*)(e + 8) = 5;
    if (Destroy__13VISpellEffect(e) != 0 || n != 3) return 2;
    if (seen[0] != e + 12 || seen[1] != e + 24 || seen[2] != e + 36) return 3;
    if (*(int*)(e + 8) != 0) return 4;
    for (int i = 0; i < 64; ++i) if ((i < 8 || i >= 12) && e[i] != 0x11) return 5;
    if (Destroy__13VISpellEffect(e) != -1 || n != 3) return 6;
    return 0;
}
