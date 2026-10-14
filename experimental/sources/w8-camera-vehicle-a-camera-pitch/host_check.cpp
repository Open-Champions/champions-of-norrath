// Host contract for GetPitch__6Camera. Not an EE plan input.
#include "get_pitch.cpp"

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
    camera[106] = 0x34; camera[107] = 0x12;
    expect(GetPitch__6Camera(camera) == 0x1234, "positive halfword");
    camera[106] = 0x00; camera[107] = 0x80;
    expect(GetPitch__6Camera(camera) == -32768, "sign extension");
    camera[106] = 0xff; camera[107] = 0xff;
    expect(GetPitch__6Camera(camera) == -1, "all ones is -1");
    camera[106] = 0; camera[107] = 0;
    camera[104] = 0x77;
    expect(GetPitch__6Camera(camera) == 0, "yaw bytes are not read");
    expect(camera[104] == 0x77 && camera[105] == 0x33, "nothing is stored");
    return failures ? 1 : 0;
}
