// lightInit__Fv at 0x0013d0f8, 312 bytes, read from the pinned SLUS-20565 image.
// Fresh compilation is not an original-byte match. No GS light has run.
//
// A three-float direction with bits 0xbf800000, 0xbf800000, and 0x3f800000 is
// scaled by rsqrt of the sum of squares and stored at the start of scene
// (0x004f4060, 56 bytes through savedScene). The later byte and word stores
// are the values below. Bytes 12 through 35 are not written. Then
// transformSceneLightTask__Fv, lightResetOptionalParams__Fv, and
// engineAddTask__FPFv_viPci(transformSceneLightTask__Fv, 12,
// "transformSceneLight", 0). The task name is the bytes at 0x00461dd0.
extern "C" {
extern unsigned char scene[];
void transformSceneLightTask__Fv();
void lightResetOptionalParams__Fv();
void engineAddTask__FPFv_viPci(void (*task)(), int order, char* name, int flag);

static char norrath_transform_scene_light_name[] = "transformSceneLight";

static unsigned norrath_float_bits(float value) {
    union {
        float value;
        unsigned bits;
    } converted;
    converted.value = value;
    return converted.bits;
}

static void norrath_store_u32(void* slot, unsigned value) {
    auto* dest = static_cast<unsigned char*>(slot);
    dest[0] = static_cast<unsigned char>(value);
    dest[1] = static_cast<unsigned char>(value >> 8);
    dest[2] = static_cast<unsigned char>(value >> 16);
    dest[3] = static_cast<unsigned char>(value >> 24);
}

static void norrath_store_f32(void* slot, float value) {
    norrath_store_u32(slot, norrath_float_bits(value));
}

void lightInit__Fv() {
    float x = -1.0f;
    float y = -1.0f;
    float z = 1.0f;
    const float inverse = 1.0f / __builtin_sqrtf(x * x + y * y + z * z);
    x *= inverse;
    y *= inverse;
    z *= inverse;
    norrath_store_f32(scene + 0, x);
    norrath_store_f32(scene + 4, y);
    norrath_store_f32(scene + 8, z);
    scene[36] = 64;
    scene[37] = 70;
    scene[38] = 80;
    scene[39] = 0;
    scene[40] = 32;
    scene[41] = 32;
    scene[42] = 32;
    scene[43] = 0;
    scene[44] = 64;
    scene[45] = 70;
    scene[46] = 80;
    scene[47] = 0;
    scene[48] = 32;
    scene[49] = 32;
    scene[50] = 32;
    scene[51] = 0;
    norrath_store_u32(scene + 52, 128u);
    transformSceneLightTask__Fv();
    lightResetOptionalParams__Fv();
    engineAddTask__FPFv_viPci(transformSceneLightTask__Fv, 12, norrath_transform_scene_light_name, 0);
}
}
