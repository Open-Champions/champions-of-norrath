// Host contract for transformSceneLightTask. Not an EE plan input.
#include "transform_scene_light.cpp"

#include <cmath>
#include <cstring>

extern "C" {
unsigned char scene[48];
unsigned char worldCameraMatrix[48];
unsigned char charDirectDir[12];
unsigned char charDirectDir2[12];
}

static int calls = 0;
static unsigned char* outs[2];
static unsigned char* matrices[2];
static unsigned char inputs[2][12];

static void put_f32(unsigned char* address, float value) {
    unsigned bits;
    std::memcpy(&bits, &value, 4);
    address[0] = static_cast<unsigned char>(bits);
    address[1] = static_cast<unsigned char>(bits >> 8);
    address[2] = static_cast<unsigned char>(bits >> 16);
    address[3] = static_cast<unsigned char>(bits >> 24);
}

static unsigned bits_of(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

extern "C" void matTransform34Vec__FP6Point3P8Matrix34T0(unsigned char* out, unsigned char* matrix,
                                                         unsigned char* in) {
    if (calls < 2) {
        outs[calls] = out;
        matrices[calls] = matrix;
        std::memcpy(inputs[calls], in, 12);
    }
    if (calls == 0) {
        put_f32(out, 3.0f);
        put_f32(out + 4, 4.0f);
        put_f32(out + 8, 5.0f);
    } else {
        put_f32(out, 8.0f);
        put_f32(out + 4, 9.0f);
        put_f32(out + 8, 10.0f);
    }
    ++calls;
}

int main() {
    std::memset(scene, 0x5a, sizeof scene);
    put_f32(scene, 3.0f);
    put_f32(scene + 4, -4.0f);
    put_f32(scene + 8, 2.0f);
    worldCameraMatrix[0] = 9;
    transformSceneLightTask__Fv();
    if (calls != 2) return 1;
    if (outs[0] != scene + 12 || outs[1] != scene + 24) return 2;
    if (matrices[0] != worldCameraMatrix || matrices[1] != worldCameraMatrix) return 3;
    if (bits_of(inputs[0]) != bits_of(scene) || bits_of(inputs[0] + 8) != bits_of(scene + 8)) return 4;

    float half = 0.0f;
    unsigned half_bits = 0x3f000000u;
    std::memcpy(&half, &half_bits, 4);
    float x = -3.0f;
    float y = 4.0f;
    float z = 2.0f * half;
    float scale = 1.0f / std::sqrt(x * x + y * y + z * z);
    unsigned char expected[12];
    put_f32(expected, x * scale);
    put_f32(expected + 4, y * scale);
    put_f32(expected + 8, z * scale);
    if (std::memcmp(inputs[1], expected, 12) != 0) return 5;

    unsigned char copied[12];
    put_f32(copied, 3.0f);
    put_f32(copied + 4, 4.0f);
    put_f32(copied + 8, -5.0f);
    if (std::memcmp(charDirectDir, copied, 12) != 0) return 6;
    if (std::memcmp(scene + 12, copied, 12) != 0) return 7;
    put_f32(copied, 8.0f);
    put_f32(copied + 4, 9.0f);
    put_f32(copied + 8, -10.0f);
    if (std::memcmp(charDirectDir2, copied, 12) != 0) return 8;
    if (std::memcmp(scene + 24, copied, 12) != 0) return 9;
    if (bits_of(scene) != bits_of(inputs[0]) || bits_of(scene + 8) != bits_of(inputs[0] + 8)) return 10;
    return 0;
}
