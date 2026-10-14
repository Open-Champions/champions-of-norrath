// Host comparison only. Not an EE build input.
#include <cstdio>
#define W9_UNCACHED_MASK 0u
extern "C" {
int* norrath_libmc_info_type_pointer_word_view;
int* norrath_libmc_info_free_pointer_word_view;
int* norrath_libmc_info_format_pointer_word_view;
int semaid_timerfunc = 11;
}
static int signalled = -1, signals;
extern "C" int iSignalSema(int s) { signalled = s; ++signals; return 0; }
#include "mce_get_info_apdx.cpp"

int main() {
    int failures = 0;
    int block[48];
    for (int i = 0; i < 48; ++i) block[i] = 1000 + i;
    int t = -1, f = -1, m = -1;
    for (int mask = 0; mask < 8; ++mask) {
        t = f = m = -1; signals = 0;
        norrath_libmc_info_type_pointer_word_view = (mask & 1) ? &t : nullptr;
        norrath_libmc_info_free_pointer_word_view = (mask & 2) ? &f : nullptr;
        norrath_libmc_info_format_pointer_word_view = (mask & 4) ? &m : nullptr;
        mceGetInfoApdx(block);
        if (t != ((mask & 1) ? 1000 : -1) || f != ((mask & 2) ? 1001 : -1) || m != ((mask & 4) ? 1036 : -1)) { std::printf("mask %d copies wrong: %d %d %d\n", mask, t, f, m); ++failures; }
        if (signals != 1 || signalled != 11) { std::printf("mask %d signal wrong\n", mask); ++failures; }
    }
    if (failures) return 2;
    std::printf("mceGetInfoApdx cases passed\n");
    return 0;
}
