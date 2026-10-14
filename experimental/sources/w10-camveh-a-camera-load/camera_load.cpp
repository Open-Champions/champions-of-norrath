// Observed Load__6CameraPc at 0x0026c5d8, 408 bytes. The inverse of Save__6CameraPc.
// Arguments: the camera and a byte buffer `in`. Returns the number of bytes
// consumed (196). Nothing here was executed on the EE.
//
// First Reset__6Cameraii(camera, 1, 0) runs. Then, reading `in` sequentially:
// three floats go to worldCameraPos (0x0037ee80); sixteen floats go to
// worldCameraMatrix (0x0037ef00) at float index 3*row+col for row and col in
// 0..3 (so neighbouring groups overlap and index 12 is the last one written);
// halfwords to camera+104 and camera+106; words to camera+84 (a) and
// camera+88 (b); floats to camera+144 (m), +212 and +140 (z); 96 bytes to
// camera+256.
// Two derived floats are then stored. With lo = the float at camera+176 and hi
// the float at camera+184: camera+216 = 1.0 - (z - lo) / (hi - lo); and with
// F(k) = the float at camera+188+4*k, camera+252 = F(b) + (F(a) - F(b)) *
// min(1.0, m). The min is the hardware min.s. Both use the values just loaded,
// not the pre-Reset ones, and the ones at +176/+184 as left by Reset.
// Nothing else is written. What the fields mean is not established here.
extern "C" {
extern float worldCameraPos[3];
extern float worldCameraMatrix[16];
void Reset__6Cameraii(void* camera, int defaults, int fresh);
}

typedef float __attribute__((may_alias)) AliasedFloat;
typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;

static inline float fp_min(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return a < b ? a : b;
#endif
}

extern "C" int Load__6CameraPc(unsigned char* camera, const unsigned char* in) {
    const unsigned char* p = in;
    Reset__6Cameraii(camera, 1, 0);
    for (int i = 0; i < 3; ++i) {
        worldCameraPos[i] = *reinterpret_cast<const AliasedFloat*>(p);
        p += 4;
    }
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            worldCameraMatrix[row * 3 + col] = *reinterpret_cast<const AliasedFloat*>(p);
            p += 4;
        }
    }
    *reinterpret_cast<AliasedShort*>(camera + 104) = *reinterpret_cast<const AliasedShort*>(p);
    p += 2;
    *reinterpret_cast<AliasedShort*>(camera + 106) = *reinterpret_cast<const AliasedShort*>(p);
    p += 2;
    const int a = *reinterpret_cast<const AliasedInt*>(p);
    p += 4;
    *reinterpret_cast<AliasedInt*>(camera + 84) = a;
    const int b = *reinterpret_cast<const AliasedInt*>(p);
    p += 4;
    *reinterpret_cast<AliasedInt*>(camera + 88) = b;
    const float m = *reinterpret_cast<const AliasedFloat*>(p);
    p += 4;
    *reinterpret_cast<AliasedFloat*>(camera + 144) = m;
    *reinterpret_cast<AliasedFloat*>(camera + 212) = *reinterpret_cast<const AliasedFloat*>(p);
    p += 4;
    const float z = *reinterpret_cast<const AliasedFloat*>(p);
    p += 4;
    *reinterpret_cast<AliasedFloat*>(camera + 140) = z;

    const float lo = *reinterpret_cast<AliasedFloat*>(camera + 176);
    const float hi = *reinterpret_cast<AliasedFloat*>(camera + 184);
    *reinterpret_cast<AliasedFloat*>(camera + 216) = 1.0f - (z - lo) / (hi - lo);
    const float fb = *reinterpret_cast<AliasedFloat*>(camera + 188 + 4 * b);
    const float fa = *reinterpret_cast<AliasedFloat*>(camera + 188 + 4 * a);
    *reinterpret_cast<AliasedFloat*>(camera + 252) = fb + (fa - fb) * fp_min(1.0f, m);

    for (int i = 0; i < 96; ++i) camera[256 + i] = *p++;
    return static_cast<int>(p - in);
}
