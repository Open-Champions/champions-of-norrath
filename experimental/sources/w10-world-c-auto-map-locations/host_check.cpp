// Host contract for the w10-world-c-auto-map-locations storage. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "auto_map_locations.cpp"

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

int main() {
    // Independent expected bit patterns of the twelve floats (the file words read from the image).
    const unsigned bits[12] = {0, 0, 0x43480000u, 0xc2f00000u, 0x43340000u, 0xc2f00000u, 0, 0, 0, 0xc2f00000u, 0, 0xc30c0000u};
    unsigned got[12];
    std::memcpy(got, loc, sizeof got);
    expect(std::memcmp(got, bits, sizeof bits) == 0, "twelve floats match the observed words");
    expect(sizeof(loc) == 48, "48 bytes");
    if (failures) return 1;
    std::printf("w10-world-c-auto-map-locations contract passed\n");
    return 0;
}
