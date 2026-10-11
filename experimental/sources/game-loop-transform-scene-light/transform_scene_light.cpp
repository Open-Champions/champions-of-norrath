// Observed transformSceneLightTask at 0x0013c918, 292 bytes.
// The first matTransform34Vec writes scene+12 from the 12-byte vector at
// scene through worldCameraMatrix. The float at scene+20 is then negated.
// Those 12 bytes are copied to charDirectDir. The vector passed to the
// second transform is the original scene vector with x and y negated and z
// multiplied by the 0x3f000000 constant, then scaled by 1/sqrt of the sum of
// squares. rsqrt.s is that quotient with a 1 dividend. The second transform
// writes scene+24. The float at scene+32 is negated, and those 12 bytes are
// copied to charDirectDir2.
extern "C" void matTransform34Vec__FP6Point3P8Matrix34T0(unsigned char* out, unsigned char* matrix,
                                                         unsigned char* in);
extern "C" unsigned char scene[];
extern "C" unsigned char worldCameraMatrix[];
extern "C" unsigned char charDirectDir[];
extern "C" unsigned char charDirectDir2[];

namespace {
unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

float load_f32(const unsigned char* address) {
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = load_u32(address);
    return value.as_float;
}

void store_f32(unsigned char* address, float value) {
    union {
        unsigned as_bits;
        float as_float;
    } bits;
    bits.as_float = value;
    store_u32(address, bits.as_bits);
}

void copy12(unsigned char* destination, const unsigned char* source) {
    for (int byte = 0; byte < 12; ++byte) destination[byte] = source[byte];
}

float half() {
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = 0x3f000000u;
    return value.as_float;
}
}

extern "C" void transformSceneLightTask__Fv() {
    matTransform34Vec__FP6Point3P8Matrix34T0(scene + 12, worldCameraMatrix, scene);
    float x = -load_f32(scene);
    float y = -load_f32(scene + 4);
    float z = load_f32(scene + 8) * half();
    store_f32(scene + 20, -load_f32(scene + 20));
    copy12(charDirectDir, scene + 12);
    float scale = 1.0f / __builtin_sqrtf(x * x + y * y + z * z);
    unsigned char directed[12];
    store_f32(directed, x * scale);
    store_f32(directed + 4, y * scale);
    store_f32(directed + 8, z * scale);
    matTransform34Vec__FP6Point3P8Matrix34T0(scene + 24, worldCameraMatrix, directed);
    store_f32(scene + 32, -load_f32(scene + 32));
    copy12(charDirectDir2, scene + 24);
}
