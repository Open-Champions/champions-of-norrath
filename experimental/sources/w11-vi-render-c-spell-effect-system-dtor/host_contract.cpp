#include "dtor.cpp"

static void* seen[8];
static int arg[8];
static int kind[8];
static int n;
extern "C" void _$_t6VIPool1ZP13VISpellEffect(void* p, int f) { kind[n] = 1; arg[n] = f; seen[n++] = p; }
extern "C" void _$_7VILList(void* p, int f) { kind[n] = 2; arg[n] = f; seen[n++] = p; }
extern "C" void __builtin_delete(void* p) { kind[n] = 3; arg[n] = 0; seen[n++] = p; }

int main() {
    alignas(8) static char object[1200];
    const int all_flags[] = {0, 1, 2, 3, 0x10001, -1};
    for (int flags : all_flags) {
        n = 0;
        _$_19VISpellEffectSystem(object, flags);
        int expect = (flags & 1) ? 4 : 3;
        if (n != expect) return 1;
        if (kind[0] != 1 || seen[0] != object + 1112 || arg[0] != 2) return 2;
        if (kind[1] != 2 || seen[1] != object + 112 || arg[1] != 2) return 3;
        if (kind[2] != 2 || seen[2] != object + 100 || arg[2] != 2) return 4;
        if ((flags & 1) && (kind[3] != 3 || seen[3] != object)) return 5;
    }
    return 0;
}
