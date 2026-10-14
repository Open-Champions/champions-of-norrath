/* Observed lightVU0StoppedProcessing__Fv at 0x0013ce70, 648 bytes in the
 * pinned image. It calls lightResetOptionalParams__Fv on entry and again on
 * exit, and returns nothing. The VU0 coprocessor data-memory words at
 * 0x1100ffc0, 0x1100ffc4, 0x1100ffc8, 0x1100ffcc, 0x1100ffd0, 0x1100ffd4,
 * 0x1100ffd8, 0x1100ffdc, and 0x1100ffac receive single-precision values:
 * 8388608.0 at the four 0x1100ffd0 words, the unnamed .lit4 float 8388736.0
 * at 0x1100ffcc, the float conversion of the scene word at offset 52 biased
 * by 0x80000000 at the three 0x1100ffc0 words, and worldCameraPos[2] scaled
 * by 0.25 at 0x1100ffac. Four column lengths of worldCharViewMatrix are
 * computed through sqrt and stored to dead stack slots; nothing reads them.
 * The eight scene bytes at offsets 44 through 51 are then scaled by 1.0, or
 * by the globalCharLightScale word when interfaceMode is nonzero, converted
 * to integer with round-to-nearest, clamped to the 0..255 range, and stored
 * at scene offsets 36 through 43. scene, worldCharViewMatrix,
 * worldCameraPos, interfaceMode, and globalCharLightScale stay imports.
 * Fresh compilation is not an original-byte match. FCSR rounding mode,
 * VU0/converter effects, and the meaning of the scene words are unknown.
 */
extern "C" void lightResetOptionalParams__Fv();
extern "C" unsigned char scene[];
extern "C" float worldCharViewMatrix[];
extern "C" float worldCameraPos[];
extern "C" int interfaceMode;
extern "C" int globalCharLightScale;

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_store_float(unsigned address, float value);
#define NORRATH_STORE_FLOAT(address, value) norrath_store_float(address, value)
#else
static inline void norrath_store_float(unsigned address, float value) {
    *reinterpret_cast<volatile float*>(static_cast<unsigned long>(address)) = value;
}
#define NORRATH_STORE_FLOAT(address, value) norrath_store_float(address, value)
#endif

static int float_to_int_word(float value) {
    // Round to nearest, ties to even, matching cvt.w.s under the default
    // round-to-nearest FCSR mode, with saturation instead of C++ UB.
    if (value > 2147483520.0f) return 0x7fffffff;
    if (value <= -2147483648.0f) return static_cast<int>(0x80000000u);
    const float shifted = value >= 0.0f ? value + 0.5f : value - 0.5f;
    int truncated = static_cast<int>(shifted);
    if (value > -16777216.0f && value < 16777216.0f) {
        const float doubled = value + value;
        const int doubled_int = static_cast<int>(doubled);
        if (static_cast<float>(doubled_int) == doubled && (doubled_int & 1) != 0 &&
            (truncated & 1) != 0)
            truncated += (value >= 0.0f ? -1 : 1);
    }
    return truncated;
}

extern "C" void lightVU0StoppedProcessing__Fv() {
    lightResetOptionalParams__Fv();
    NORRATH_STORE_FLOAT(0x1100ffccu, 8388736.0f);
    unsigned scene_word = static_cast<unsigned>(scene[52])
        | (static_cast<unsigned>(scene[53]) << 8)
        | (static_cast<unsigned>(scene[54]) << 16)
        | (static_cast<unsigned>(scene[55]) << 24);
    const float biased =
        static_cast<float>(static_cast<int>(scene_word + 0x80000000u));
    NORRATH_STORE_FLOAT(0x1100ffc8u, biased);
    NORRATH_STORE_FLOAT(0x1100ffc0u, biased);
    NORRATH_STORE_FLOAT(0x1100ffc4u, biased);
    NORRATH_STORE_FLOAT(0x1100ffdcu, 8388608.0f);
    NORRATH_STORE_FLOAT(0x1100ffd0u, 8388608.0f);
    NORRATH_STORE_FLOAT(0x1100ffd4u, 8388608.0f);
    NORRATH_STORE_FLOAT(0x1100ffd8u, 8388608.0f);
    for (int column = 0; column < 4; ++column) {
        const float x = worldCharViewMatrix[column];
        const float y = worldCharViewMatrix[4 + column];
        const float z = worldCharViewMatrix[8 + column];
        const volatile float dead_length = __builtin_sqrtf(x * x + y * y + z * z);
        (void)dead_length;
    }
    NORRATH_STORE_FLOAT(0x1100ffacu, worldCameraPos[2] * 0.25f);
    float scale = 1.0f;
    if (interfaceMode != 0) {
        float bits;
        const int word = globalCharLightScale;
        __builtin_memcpy(&bits, &word, sizeof(bits));
        scale = bits;
    }
    for (int index = 0; index < 8; ++index) {
        const float scaled = static_cast<float>(scene[44 + index]) * scale;
        int clamped = float_to_int_word(scaled);
        if (clamped > 255) clamped = 255;
        if (clamped < 0) clamped = 0;
        scene[36 + index] = static_cast<unsigned char>(clamped);
    }
    lightResetOptionalParams__Fv();
}
