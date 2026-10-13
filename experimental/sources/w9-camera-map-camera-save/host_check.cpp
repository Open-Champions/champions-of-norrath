// Host contract for Save__6CameraPc. Not an EE plan input.
#include "save.cpp"

#include <cstdio>
#include <cstring>

float worldCameraPos[3];
float worldCameraMatrix[16];

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    alignas(4) unsigned char camera[400];
    alignas(4) unsigned char out[260];
    for (int i = 0; i < 400; ++i) camera[i] = static_cast<unsigned char>(i * 7 + 1);
    for (int i = 0; i < 12; ++i) reinterpret_cast<unsigned char*>(worldCameraPos)[i] = static_cast<unsigned char>(0xa0 + i);
    // Distinct recognizable floats in the matrix: float k holds the bits 0x40000000 + k.
    for (int k = 0; k < 16; ++k) {
        unsigned v = 0x40000000u + k;
        std::memcpy(reinterpret_cast<unsigned char*>(worldCameraMatrix) + 4 * k, &v, 4);
    }
    unsigned char camera_before[400];
    std::memcpy(camera_before, camera, sizeof camera);
    std::memset(out, 0xee, sizeof out);
    int n = Save__6CameraPc(camera, out);
    expect(n == 196, "returns 196 bytes");
    expect(std::memcmp(camera, camera_before, sizeof camera) == 0, "camera unchanged");
    expect(std::memcmp(out, worldCameraPos, 12) == 0, "position first");
    // Matrix groups start at float 0, 3, 6, 9 and take four floats each.
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            expect(std::memcmp(out + 12 + 16 * row + 4 * col, reinterpret_cast<unsigned char*>(worldCameraMatrix) + 4 * (row * 3 + col), 4) == 0,
                   "overlapping matrix groups");
    expect(std::memcmp(out + 76, camera + 104, 2) == 0, "yaw halfword");
    expect(std::memcmp(out + 78, camera + 106, 2) == 0, "pitch halfword");
    expect(std::memcmp(out + 80, camera + 84, 4) == 0, "word 84");
    expect(std::memcmp(out + 84, camera + 88, 4) == 0, "word 88");
    expect(std::memcmp(out + 88, camera + 144, 4) == 0, "float 144");
    expect(std::memcmp(out + 92, camera + 212, 4) == 0, "float 212");
    expect(std::memcmp(out + 96, camera + 140, 4) == 0, "float 140");
    expect(std::memcmp(out + 100, camera + 256, 96) == 0, "96 bytes from 256");
    expect(out[196] == 0xee, "nothing written past 196");
    return failures ? 1 : 0;
}
