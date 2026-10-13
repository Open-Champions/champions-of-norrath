// Host check for floorGetSaveInfo__FPci. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off -fno-builtin host_check.cpp
// Cells are filled with pseudo-random records. The expected output is computed here with an
// independent walk (all z, row, col positions collected first, then the limit applied to the
// prefix) and compared byte for byte except byte 11 of each record, which the original takes
// from stale stack and this source writes as zero.
#include "floor_get_save_info.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" {
unsigned char* sprGrid[30][30];
}
static int iatanCalls;
extern "C" int iatan2__Fff(float y, float x) {
    ++iatanCalls;
    return static_cast<int>(y * 3.0f + x * 5.0f) + 7;
}

namespace {
alignas(8) unsigned char cells[40][800];
unsigned lcg = 99;
unsigned rnd() { lcg = lcg * 1664525u + 1013904223u; return lcg >> 8; }

void fill(int n, int maxCount) {
    std::memset(sprGrid, 0, sizeof sprGrid);
    for (int k = 0; k < n; ++k) {
        for (auto& b : cells[k]) b = static_cast<unsigned char>(rnd());
        for (int z = 0; z < 31; ++z) {
            const int m = (rnd() & 1) ? 400 : 32000;
            short v[3] = {static_cast<short>(static_cast<int>(rnd() % (2 * m)) - m), static_cast<short>(static_cast<int>(rnd() % (2 * m)) - m),
                          static_cast<short>(static_cast<int>(rnd() % 2000) - 1000)};
            std::memcpy(cells[k] + 24 * z + 16, v, 6);
        }
        int count = static_cast<int>(rnd() % (maxCount + 1));
        if (k % 7 == 3) count = -2;
        std::memcpy(cells[k] + 760, &count, 4);
        sprGrid[(k * 7) % 30][(k * 11 + 3) % 30] = cells[k];
    }
}

int run(const char* what, int n, int maxCount, int limit) {
    fill(n, maxCount);
    alignas(8) static unsigned char out[1 << 17], want[1 << 17];
    std::memset(out, 0xaa, sizeof out);
    std::memset(want, 0xaa, sizeof want);
    iatanCalls = 0;
    const int got = floorGetSaveInfo__FPci(reinterpret_cast<char*>(out), limit);
    // expected
    int bytes = 8, count = 0;
    if (limit >= 21) {
        bool stop = false;
        for (int z = 0; z < 31 && !stop; ++z)
            for (int row = 0; row < 30 && !stop; ++row)
                for (int col = 0; col < 30 && !stop; ++col) {
                    unsigned char* c = sprGrid[row][col];
                    if (!c) continue;
                    int cnt;
                    std::memcpy(&cnt, c + 760, 4);
                    if (cnt <= z) continue;
                    const unsigned char* r = c + 24 * z;
                    short y, x, t;
                    std::memcpy(&y, r + 16, 2); std::memcpy(&x, r + 18, 2); std::memcpy(&t, r + 20, 2);
                    unsigned short rec[6];
                    std::memcpy(&rec[0], r + 8, 2); std::memcpy(&rec[1], r + 10, 2); std::memcpy(&rec[2], r + 12, 2);
                    const int sum = x * x + y * y;
                    rec[3] = static_cast<unsigned short>(static_cast<int>(std::sqrt(static_cast<float>(sum)) + 0.5f));
                    rec[4] = static_cast<unsigned short>(static_cast<int>(static_cast<float>(y) * 3.0f + static_cast<float>(x) * 5.0f) + 7);
                    rec[5] = static_cast<unsigned short>((((t - 979) >> 1) & 0xff));
                    std::memcpy(want + bytes, rec, 10);
                    want[bytes + 10] = rec[5] & 0xff;
                    want[bytes + 11] = 0;
                    bytes += 12;
                    ++count;
                    if (limit - 256 < bytes) stop = true;
                }
    }
    std::memcpy(want, &count, 4);
    if (got != bytes) { std::printf("%s: size %d want %d\n", what, got, bytes); return 1; }
    if (iatanCalls != count) { std::printf("%s: iatan2 calls %d want %d\n", what, iatanCalls, count); return 2; }
    if (std::memcmp(out, want, sizeof out) != 0) {
        for (size_t i = 0; i < sizeof out; ++i)
            if (out[i] != want[i]) { std::printf("%s: byte %zu got %u want %u\n", what, i, out[i], want[i]); break; }
        return 3;
    }
    return 0;
}
}

int main() {
    int rc = 0;
    rc |= run("empty", 0, 5, 1000);
    rc |= run("limit 20", 5, 31, 20);
    rc |= run("limit 21", 5, 31, 21);
    rc |= run("limit 22", 5, 31, 22);
    rc |= run("limit negative", 5, 31, -4);
    rc |= run("tiny stop", 5, 31, 268);   // 268 - 256 = 12: stops after the first record
    rc |= run("stop later", 5, 31, 269);  // after the second record
    rc |= run("few", 3, 3, 5000);
    rc |= run("many", 40, 31, 60000);
    rc |= run("capped", 40, 31, 700);
    rc |= run("zero counts", 20, 0, 5000);
    for (int i = 0; i < 20; ++i) rc |= run("random", 1 + (i * 2) % 40, 31, 21 + static_cast<int>(rnd() % 3000));
    if (rc) return rc;
    std::puts("floor get save info passed");
    return 0;
}
