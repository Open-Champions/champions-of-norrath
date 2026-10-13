#include "dtor.cpp"

static void* ptr[8];
static int kind[8];
static int flag[8];
static int n;
static void rec(int k, void* p, int f) { kind[n] = k; ptr[n] = p; flag[n] = f; ++n; }
extern "C" void _$_t6VIPool1ZP20VIParticleDefinition(void* p, int f) { rec(1, p, f); }
extern "C" void _$_7VILList(void* p, int f) { rec(2, p, f); }
extern "C" void __builtin_delete(void* p) { rec(3, p, 0); }

int main() {
    unsigned char object[160];
    for (int flags = 0; flags < 4; ++flags) {
        n = 0;
        _$_16VIParticleSystem(object, flags);
        if (n != ((flags & 1) ? 4 : 3)) return 1 + flags;
        if (kind[0] != 1 || ptr[0] != object + 84 || flag[0] != 2) return 10;
        if (kind[1] != 2 || ptr[1] != object + 64 || flag[1] != 2) return 11;
        if (kind[2] != 2 || ptr[2] != object + 52 || flag[2] != 2) return 12;
        if ((flags & 1) && (kind[3] != 3 || ptr[3] != object)) return 13;
    }
    return 0;
}
