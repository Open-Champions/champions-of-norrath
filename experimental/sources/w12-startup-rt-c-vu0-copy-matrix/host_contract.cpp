// Host check only; not an EE build input. Build the source with -DsceVu0CopyMatrix=w12_cm.
#include <cstdio>
#include <cstring>
extern "C" {
static int dintr_ret, order[8], n;
int DIntr() { order[n++] = 1; return dintr_ret; }
int EIntr() { order[n++] = 2; return 0; }
void w12_cm(float*, const float*);
}
int main() {
    alignas(16) float src[16], dst[16];
    for (int i = 0; i < 16; ++i) { src[i] = float(i + 1); dst[i] = -1.0f; }
    dintr_ret = 1; n = 0;
    w12_cm(dst, src);
    if (std::memcmp(src, dst, sizeof src)) return 1;
    if (n != 2 || order[0] != 1 || order[1] != 2) return 2;
    dintr_ret = 0; n = 0;
    for (int i = 0; i < 16; ++i) dst[i] = 0;
    w12_cm(dst, src);
    if (std::memcmp(src, dst, sizeof src) || n != 1 || order[0] != 1) return 3;
    alignas(16) float both[20];
    for (int i = 0; i < 20; ++i) both[i] = float(i);
    n = 0; w12_cm(both + 4, both);              // overlapping: behaves as a snapshot copy
    for (int i = 0; i < 16; ++i) if (both[4 + i] != float(i)) return 4;
    if (both[0] != 0.0f || both[3] != 3.0f) return 5;
    std::puts("sceVu0CopyMatrix ok");
    return 0;
}
