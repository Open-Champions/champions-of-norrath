#include "world_dtor.cpp"

static void* seen[8];
static int arg[8];
static int kind[8];
static int n;
extern "C" void _$_t7VIArray1Z13VIWorldRegion(void* p, int f) { kind[n] = 1; arg[n] = f; seen[n++] = p; }
extern "C" void _$_t7VIArray1Z6VIZone(void* p, int f) { kind[n] = 2; arg[n] = f; seen[n++] = p; }
extern "C" void _$_11VIWorldTree(void* p, int f) { kind[n] = 3; arg[n] = f; seen[n++] = p; }
extern "C" void __builtin_delete(void* p) { kind[n] = 4; arg[n] = 0; seen[n++] = p; }

int main() {
    alignas(8) static char object[128];
    const int all_flags[] = {0, 1, 2, 3, 0x10001, -1, -2};
    for (int flags : all_flags) {
        n = 0;
        _$_7VIWorld(object, flags);
        if (n != ((flags & 1) ? 4 : 3)) return 1;
        if (kind[0] != 1 || seen[0] != object + 80 || arg[0] != 2) return 2;
        if (kind[1] != 2 || seen[1] != object + 72 || arg[1] != 2) return 3;
        if (kind[2] != 3 || seen[2] != object + 52 || arg[2] != 2) return 4;
        if ((flags & 1) && (kind[3] != 4 || seen[3] != object)) return 5;
    }
    return 0;
}
