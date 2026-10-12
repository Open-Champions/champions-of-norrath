// Host contract for updateTrails__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" {
unsigned char trail[64 * 1040] = {0};
void updateTrails__Fv();
}
static int failures = 0;
static void expect(bool ok, const char* what) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; }
}
static unsigned char* rec(int n) { return trail + n * 1040; }
static void set16(unsigned char* r, unsigned off, unsigned v) { r[off] = v & 0xff; r[off + 1] = (v >> 8) & 0xff; }
static unsigned get16(const unsigned char* r, unsigned off) { return r[off] | (r[off + 1] << 8); }
static void stamp(unsigned char* r, unsigned n, int v) { set16(r, 6 + n * 8, static_cast<unsigned>(v) & 0xffff); }
static void setup(int n, unsigned count, unsigned age, unsigned start, unsigned end, unsigned flags, int life) {
    unsigned char* r = rec(n);
    std::memset(r, 0, 1040);
    set16(r, 1024, count); set16(r, 1026, age); r[1028] = start; r[1029] = end; r[1031] = flags;
    set16(r, 1032, static_cast<unsigned>(life) & 0xffff);
}

int main() {
    // 0: unused record is left alone, age included.
    setup(0, 0, 5, 0, 3, 0, 4);
    // 1: bit 0 skips; 2: bit 4 stops the age advancing.
    setup(1, 1, 5, 0, 3, 1, 4);
    setup(2, 1, 5, 0, 3, 0x10, 100);
    // 3: start equals end clears the count.
    setup(3, 1, 5, 7, 7, 0, 4);
    // 4: step 1, stamps 0..4 at entries 0..4, end 4, age 5 -> 6, lifetime 4. Differences
    // 6, 5 and 4 are not below 4, so entries 0, 1 and 2 retire; 6 - 3 = 3 stops the scan.
    setup(4, 1, 5, 0, 4, 0, 4);
    for (unsigned n = 0; n < 5; ++n) stamp(rec(4), n, static_cast<int>(n));
    // 5: step 2 (bit 1). Entries 0, 2, 4 are visited; stamps all 0, life 100: nothing retires.
    setup(5, 1, 0, 0, 4, 2, 100);
    // 6: everything expires and the scan reaches end: count cleared, start == end.
    setup(6, 9, 50, 0, 3, 0, 4);
    // 7: step 2 expiring: 0 -> 2 -> end 4. Count cleared.
    setup(7, 9, 50, 0, 4, 2, 4);
    // 8: negative lifetime and wrapped stamp compare as signed halfwords.
    setup(8, 1, 0, 0, 6, 0, -2);
    stamp(rec(8), 0, 0xfffe);   // -2: age 1 - (-2) = 3, not below -2 -> retire; next stamp 0: 1 - 0 = 1, not below -2.
    // 9: start wraps at 128 when stepping.
    setup(9, 1, 50, 127, 1, 0, 4);
    // 10: age wraps at 16 bits.
    setup(10, 1, 0xffff, 0, 0, 0, 4);

    updateTrails__Fv();

    expect(get16(rec(0), 1026) == 5 && get16(rec(0), 1024) == 0, "unused untouched");
    expect(get16(rec(1), 1026) == 5 && get16(rec(1), 1024) == 1, "bit 0 untouched");
    expect(get16(rec(2), 1026) == 5, "bit 4 keeps the age");
    expect(get16(rec(3), 1026) == 6 && get16(rec(3), 1024) == 0, "start == end clears and still ages");
    expect(rec(4)[1028] == 3 && get16(rec(4), 1024) == 1 && get16(rec(4), 1026) == 6, "step 1 retires entries 0..2");
    expect(rec(5)[1028] == 0 && get16(rec(5), 1024) == 1, "nothing retires");
    expect(rec(6)[1028] == 3 && get16(rec(6), 1024) == 0, "scan reaches end and clears");
    expect(rec(7)[1028] == 4 && get16(rec(7), 1024) == 0, "step 2 reaches end");
    expect(rec(8)[1028] == 6 && get16(rec(8), 1024) == 0, "signed compares: negative lifetime retires to end");
    expect(rec(9)[1028] == 1 && get16(rec(9), 1024) == 0, "wrap at 128");
    expect(get16(rec(10), 1026) == 0, "16-bit age wrap");
    expect(get16(rec(11), 1026) == 0, "later records untouched");
    if (failures) return 1;
    std::printf("update-trails contract passed\n");
    return 0;
}
