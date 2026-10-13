// Host contract for P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct Point3 { float x, y, z; };
extern "C" {
unsigned char lineBuffer[128 * 36];
unsigned nmLines;
void P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf(Point3, Point3, unsigned char, unsigned char, unsigned char, unsigned char,
                                          unsigned char, unsigned char, unsigned char, unsigned char, float);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    std::memset(lineBuffer, 0xee, sizeof lineBuffer);
    nmLines = 5;
    P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf({1, 2, 3}, {4, 5, 6}, 10, 11, 12, 13, 14, 15, 16, 17, 0.5f);
    const unsigned char* r = lineBuffer + 5 * 36;
    float v[3]; std::memcpy(v, r, 12);
    expect(v[0] == 1 && v[1] == 2 && v[2] == 3, "first point");
    std::memcpy(v, r + 12, 12);
    expect(v[0] == 4 && v[1] == 5 && v[2] == 6, "second point");
    for (int i = 0; i < 8; ++i) expect(r[24 + i] == 10 + i, "byte order");
    float f; std::memcpy(&f, r + 32, 4);
    expect(f == 0.5f, "float");
    expect(nmLines == 6, "count incremented");
    expect(lineBuffer[4 * 36 + 35] == 0xee && lineBuffer[6 * 36] == 0xee, "neighbours untouched");
    nmLines = 128;
    std::memset(lineBuffer, 0xee, sizeof lineBuffer);
    P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf({1, 2, 3}, {4, 5, 6}, 1, 2, 3, 4, 5, 6, 7, 8, 1.0f);
    expect(nmLines == 128 && lineBuffer[127 * 36] == 0xee, "full buffer ignored");
    nmLines = 127;
    P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf({1, 2, 3}, {4, 5, 6}, 1, 2, 3, 4, 5, 6, 7, 8, 1.0f);
    expect(nmLines == 128 && lineBuffer[127 * 36 + 24] == 1, "last slot used");
    if (failures) return 1;
    std::printf("P_Line contract passed\n");
    return 0;
}
