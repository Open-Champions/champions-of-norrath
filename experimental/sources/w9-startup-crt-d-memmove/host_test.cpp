// Host comparison only. Not an EE build input. Compares every overlap offset and
// length against an independent temporary-buffer model, and checks the return.
#include <cstdio>
#include <cstring>
#define memmove w9_memmove_under_test
#include "memmove.cpp"
#undef memmove

int main() {
    int failures = 0;
    for (unsigned count = 0; count <= 40; ++count) {
        for (int dst = 0; dst <= 48; ++dst) {
            for (int src = 0; src <= 48; ++src) {
                unsigned char a[160], b[160], tmp[64];
                for (int i = 0; i < 160; ++i) a[i] = b[i] = static_cast<unsigned char>(i * 7 + 3);
                std::memcpy(tmp, b + src, count);
                std::memcpy(b + dst, tmp, count);
                void* r = w9_memmove_under_test(a + dst, a + src, count);
                if (r != a + dst || std::memcmp(a, b, sizeof a) != 0) {
                    if (failures++ < 5) std::printf("mismatch count=%u dst=%d src=%d\n", count, dst, src);
                }
            }
        }
    }
    if (failures) return 2;
    std::printf("memmove cases passed\n");
    return 0;
}
