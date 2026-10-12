// Host contract for table. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char table[32768];
int main() {
    if (sizeof table != 32768u) return 1;
    for (unsigned i = 0; i < sizeof table; ++i) {
        if (table[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "table", i);
            return 1;
        }
    }
    table[0] = 1;
    if (table[0] != 1) return 1;
    return 0;
}
