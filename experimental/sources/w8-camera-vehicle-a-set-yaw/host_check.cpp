// Host contract for SetYaw__6Camerai. Not an EE plan input.
#include "set_yaw.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    unsigned char camera[160];
    unsigned char before[160];
    std::memset(camera, 0x33, sizeof camera);
    std::memcpy(before, camera, sizeof camera);
    SetYaw__6Camerai(camera, 0x12345678);
    expect(camera[104] == 0x78 && camera[105] == 0x56, "low halfword little endian");
    before[104] = 0x78; before[105] = 0x56;
    expect(std::memcmp(camera, before, sizeof camera) == 0, "no other byte changes");
    SetYaw__6Camerai(camera, -2);
    expect(camera[104] == 0xfe && camera[105] == 0xff, "negative value");
    before[104] = 0xfe; before[105] = 0xff;
    expect(std::memcmp(camera, before, sizeof camera) == 0, "still no other byte changes");
    return failures ? 1 : 0;
}
