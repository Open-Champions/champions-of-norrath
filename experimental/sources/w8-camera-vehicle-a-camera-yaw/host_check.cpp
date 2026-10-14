// Host contract for GetYaw__6Camera. Not an EE plan input.
#include "get_yaw.cpp"

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
    std::memset(camera, 0x33, sizeof camera);
    camera[104] = 0x34; camera[105] = 0x12;
    expect(GetYaw__6Camera(camera) == 0x1234, "positive halfword");
    camera[104] = 0x00; camera[105] = 0x80;
    expect(GetYaw__6Camera(camera) == -32768, "sign extension");
    camera[104] = 0xff; camera[105] = 0xff;
    expect(GetYaw__6Camera(camera) == -1, "all ones is -1");
    camera[104] = 0; camera[105] = 0;
    camera[106] = 0x77;
    expect(GetYaw__6Camera(camera) == 0, "pitch bytes are not read");
    expect(camera[106] == 0x77 && camera[103] == 0x33, "nothing is stored");
    return failures ? 1 : 0;
}
