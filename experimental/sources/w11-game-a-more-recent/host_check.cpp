// Host contract for moreRecent; not an EE input. Compares against a plain reference on stamps.
#include "more_recent.cpp"
#include <cstdio>
#include <cstring>
unsigned char MCA_DirContents[64 * 4];
static int reference(const unsigned char* a, const unsigned char* b) {
    unsigned long long ka = (static_cast<unsigned long long>(a[14] | (a[15] << 8)) << 40) | (static_cast<unsigned long long>(a[13]) << 32) |
                            (static_cast<unsigned long long>(a[12]) << 24) | (a[11] << 16) | (a[10] << 8) | a[9];
    unsigned long long kb = (static_cast<unsigned long long>(b[14] | (b[15] << 8)) << 40) | (static_cast<unsigned long long>(b[13]) << 32) |
                            (static_cast<unsigned long long>(b[12]) << 24) | (b[11] << 16) | (b[10] << 8) | b[9];
    return ka >= kb;
}
int main() {
    int bad = 0;
    unsigned seed = 7;
    for (int iter = 0; iter < 20000; ++iter) {
        for (int i = 0; i < 128; ++i) {
            seed = seed * 1664525u + 1013904223u;
            // small value range so ties in every field are common
            MCA_DirContents[64 + i] = static_cast<unsigned char>((seed >> 24) % 3);
        }
        MCA_DirContents[64 + 15] = 0;
        MCA_DirContents[128 + 15] = 0;
        if (iter % 5 == 0) memcpy(MCA_DirContents + 128, MCA_DirContents + 64, 64);
        if (iter % 7 == 0) MCA_DirContents[128 + 14 - (iter / 7) % 6] = MCA_DirContents[64 + 14 - (iter / 7) % 6];
        int got = moreRecent__Fii(1, 2);
        if (got != reference(MCA_DirContents + 64, MCA_DirContents + 128)) { std::printf("fail: iter %d\n", iter); bad = 1; break; }
    }
    // a halfword year that differs only in its high byte must still decide
    memset(MCA_DirContents, 0, sizeof MCA_DirContents);
    MCA_DirContents[64 + 15] = 1;
    if (moreRecent__Fii(1, 2) != 1 || moreRecent__Fii(2, 1) != 0) { std::puts("fail: year high byte"); bad = 1; }
    if (!bad) std::puts("moreRecent ok");
    return bad;
}
