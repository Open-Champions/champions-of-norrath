// Host contract for addhour. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// adddate is scripted to count calls; the contract checks the increment, the wrap at 24 with the tail call,
// and that no other record byte is touched.
#include <cstdio>
static int dateCalls;
extern "C" void adddate(unsigned char*) { ++dateCalls; }
#include "addhour.cpp"
int main() {
    int bad = 0;
    for (int h = 0; h < 256; ++h) {
        unsigned char c[8] = {1, 2, 3, (unsigned char)h, 5, 6, 7, 8};
        dateCalls = 0;
        addhour(c);
        unsigned char want = h == 23 ? 0 : (unsigned char)(h + 1);
        if (c[3] != want) { std::printf("hour %d -> %d\n", h, c[3]); ++bad; }
        if (dateCalls != (h == 23 ? 1 : 0)) { std::printf("date calls at %d: %d\n", h, dateCalls); ++bad; }
        if (c[0] != 1 || c[1] != 2 || c[2] != 3 || c[4] != 5 || c[5] != 6 || c[6] != 7 || c[7] != 8) { std::printf("other byte changed at %d\n", h); ++bad; }
    }
    return bad;
}
