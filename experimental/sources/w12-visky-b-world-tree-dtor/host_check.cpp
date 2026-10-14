#include "dtor.cpp"

static int kind[8];
static void* seen[8];
static int arg[8];
static int n;
extern "C" void Clear__11VIWorldTree(void* p) { kind[n] = 1; arg[n] = 0; seen[n++] = p; }
extern "C" void _$_t7VIArray1Z11VIWorldLeaf(void* p, int f) { kind[n] = 2; arg[n] = f; seen[n++] = p; }
extern "C" void _$_t7VIArray1Z11VIWorldNode(void* p, int f) { kind[n] = 3; arg[n] = f; seen[n++] = p; }
extern "C" void __builtin_delete(void* p) { kind[n] = 4; arg[n] = 0; seen[n++] = p; }

int main() {
    alignas(8) static char object[64];
    const int flag_values[] = {0, 1, 2, 3, 0x10001, -1, -2};
    for (int flags : flag_values) {
        n = 0;
        _$_11VIWorldTree(object, flags);
        if (n != ((flags & 1) ? 4 : 3)) return 1;
        if (kind[0] != 1 || seen[0] != object) return 2;
        if (kind[1] != 2 || seen[1] != object + 8 || arg[1] != 2) return 3;
        if (kind[2] != 3 || seen[2] != object || arg[2] != 2) return 4;
        if ((flags & 1) && (kind[3] != 4 || seen[3] != object)) return 5;
    }
    return 0;
}
