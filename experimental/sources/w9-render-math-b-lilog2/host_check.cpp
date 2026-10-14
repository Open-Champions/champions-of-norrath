// Host contract for lilog2__Fl. Not an EE plan input.
#include <cstdio>
extern "C" int lilog2__Fl(long);
int main() {
    int failures = 0;
    auto expect = [&](long v, int want) {
        const int got = lilog2__Fl(v);
        if (got != want) { std::fprintf(stderr, "fail: lilog2(%ld) = %d, want %d\n", v, got, want); ++failures; }
    };
    expect(-1, 0); expect(-100000, 0); expect(-2147483647L - 1, 0);
    expect(0, 0); expect(1, 0); expect(2, 1); expect(3, 1); expect(4, 2); expect(7, 2); expect(8, 3);
    expect(1023, 9); expect(1024, 10); expect(0x7fffffffL, 30); expect(0x40000000L, 30); expect(0x3fffffffL, 29);
    if (failures) return 1;
    std::printf("lilog2 contract passed\n");
    return 0;
}
