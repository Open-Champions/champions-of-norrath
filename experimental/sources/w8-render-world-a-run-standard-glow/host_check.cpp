#include "run_standard_glow.cpp"
#include <cstdio>
#include <cstring>
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    alignas(8) unsigned char object[256];
    const short expected[10][4] = {{0, 0, 0, 0}, {30, 30, 30, 50}, {30, 15, 15, 50}, {15, 15, 30, 50}, {10, 20, 10, 50},
                                   {20, 20, 10, 50}, {20, 0, 30, 50}, {20, 20, 15, 50}, {20, 0, 30, 50}, {0, 0, 0, 0}};
    for (int kind = 0; kind < 10; ++kind) {
        std::memset(object, 0, sizeof object);
        object[156] = static_cast<unsigned char>(kind);
        runStandardGlow__8Creature(object);
        short got[4];
        std::memcpy(got, object + 140, 8);
        for (int part = 0; part < 4; ++part) if (got[part] != expected[kind][part]) { std::fprintf(stderr, "fail: kind %d part %d\n", kind, part); ++failures; }
    }
    // Higher values stay; negative values are raised; other bytes are not touched.
    std::memset(object, 0xAB, sizeof object);
    object[156] = 1;
    const short start[4] = {100, -5, 30, 49};
    std::memcpy(object + 140, start, 8);
    runStandardGlow__8Creature(object);
    short got[4];
    std::memcpy(got, object + 140, 8);
    expect(got[0] == 100 && got[1] == 30 && got[2] == 30 && got[3] == 50, "halfwords raised only when lower");
    expect(object[139] == 0xAB && object[148] == 0xAB && object[156] == 1, "neighbours untouched");
    object[156] = 0xFF;   // signed -1: out of range
    std::memcpy(object + 140, start, 8);
    runStandardGlow__8Creature(object);
    std::memcpy(got, object + 140, 8);
    expect(got[0] == 100 && got[1] == 0 && got[2] == 30 && got[3] == 49, "out of range kind: targets zero, so a negative halfword is raised to zero");
    return failures;
}
