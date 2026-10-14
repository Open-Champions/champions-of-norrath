#include "clear.cpp"
#include <string.h>

static void* arrays[16]; static int array_flags[16]; static int array_n;
static void* deleted[16]; static int delete_n;
static int order[40]; static int order_n;
extern "C" void _$_t7VIArray1Z17VIRadialFloraInst(void* a, int f) { arrays[array_n] = a; array_flags[array_n++] = f; order[order_n++] = 1; }
extern "C" void __builtin_delete(void* p) { deleted[delete_n++] = p; order[order_n++] = 2; }

int main() {
    static unsigned char blocks[9][64];
    unsigned int words[24];
    for (int i = 0; i < 24; ++i) words[i] = 0x5000 + i;
    // slots 1, 4 and 8 are live; the rest are zero
    for (int i = 0; i < 9; ++i) words[8 + i] = 0;
    int live[3] = {1, 4, 8};
    for (int k = 0; k < 3; ++k) words[8 + live[k]] = static_cast<unsigned int>(reinterpret_cast<__UINTPTR_TYPE__>(blocks[live[k]]));
    int r = Clear__19VIRadialFloraSystem(words);
    if (r != 0 || array_n != 3 || delete_n != 3) return 1;
    for (int k = 0; k < 3; ++k) {
        if (arrays[k] != blocks[live[k]] + 44 || array_flags[k] != 2 || deleted[k] != blocks[live[k]]) return 2;
    }
    int expect[] = {1, 2, 1, 2, 1, 2};
    if (order_n != 6) return 3;
    for (int i = 0; i < 6; ++i) if (order[i] != expect[i]) return 4;
    for (int i = 0; i < 24; ++i) {
        unsigned int e = (i < 3 || (i >= 8 && i < 17) || i == 17) ? 0 : 0x5000 + i;
        if (words[i] != e) return 10 + i;
    }
    return 0;
}
