// Host contract for adddate. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// Walks every valid date of years 0..99 (a year that is a multiple of four is a leap year, including 0), one
// day at a time, and compares the record after each call with an independent day counter; checks that
// bytes 0..4 and 8.. stay untouched and that 99-12-31 wraps to 00-01-01.
#include <cstdio>
#include "adddate.cpp"
static int mdays(int y, int m) {
    static const int t[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return (m == 2 && y % 4 == 0) ? 29 : t[m - 1];
}
int main() {
    int bad = 0, y = 0, m = 1, d = 1;
    for (int n = 0; n < 36525; ++n) {
        unsigned char c[10] = {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, (unsigned char)d, (unsigned char)m, (unsigned char)y, 0xb8, 0xb9};
        int ny = y, nm = m, nd = d + 1;
        if (nd > mdays(y, m)) { nd = 1; if (++nm == 13) { nm = 1; ny = (y + 1) % 100; } }
        adddate(c);
        if (c[5] != nd || c[6] != nm || c[7] != ny) { std::printf("%d-%d-%d -> %d-%d-%d\n", y, m, d, c[7], c[6], c[5]); ++bad; }
        if (c[0] != 0xa0 || c[1] != 0xa1 || c[2] != 0xa2 || c[3] != 0xa3 || c[4] != 0xa4 || c[8] != 0xb8 || c[9] != 0xb9) { std::puts("other bytes changed"); ++bad; }
        y = ny; m = nm; d = nd;
    }
    if (y != 0 || m != 1 || d != 1) { std::puts("cycle did not close after 100 years"); ++bad; }
    return bad;
}
