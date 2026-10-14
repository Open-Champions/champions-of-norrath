#include "dtor.cpp"

static void* ptr[16]; static int kind[16]; static int flag[16]; static int n;
extern "C" void _$_7VILList(void* p, int f) { kind[n] = 1; ptr[n] = p; flag[n++] = f; }
extern "C" void _$_10VIMatrix44(void* p, int f) { kind[n] = 2; ptr[n] = p; flag[n++] = f; }
extern "C" void __builtin_delete(void* p) { kind[n] = 3; ptr[n] = p; flag[n++] = 0; }

int main() {
    static unsigned char object[4600];
    static const int offsets[7] = {4464, 4448, 1408, 1344, 1280, 36, 20};
    static const int kinds[7] = {1, 1, 2, 2, 2, 1, 1};
    for (int flags = 0; flags < 4; ++flags) {
        n = 0;
        _$_12VIAtmosphere(object, flags);
        if (n != ((flags & 1) ? 8 : 7)) return 1 + flags;
        for (int i = 0; i < 7; ++i) if (kind[i] != kinds[i] || ptr[i] != object + offsets[i] || flag[i] != 2) return 10 + i;
        if ((flags & 1) && (kind[7] != 3 || ptr[7] != object)) return 20;
    }
    return 0;
}
