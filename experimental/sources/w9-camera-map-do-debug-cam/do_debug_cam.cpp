// Observed doDebugCam__6Camera at 0x0026cf70, 1376 bytes.
// Uses only the first record of the 88-byte pad table `pad` (0x0034beb8, 352
// bytes): words at +8 and +12, floats at +20, +24, +28 and +32. Names for the
// floats are not established; here they are p20, p24, p28 and p32. M is
// worldCameraMatrix (0x0037ef00, floats) and pos is worldCameraPos
// (0x0037ee80). round(x) is x + 0.5 when x >= 0 and x - 0.5 otherwise, then
// truncated to an int. The yaw halfword is camera+104 and the pitch halfword
// camera+106 (both signed).
// 1. When (word12 & 0x200) and (word8 & 0x400) are both set: the words at
//    camera+56, +60 and +152 become 0 and the word at camera+52 (the mode)
//    becomes 1; nothing else happens.
// 2. Otherwise the mode at camera+52 selects:
//    mode 3: yaw = round(yaw + p28 * 400.0); pitch = round(pitch + p32 *
//      400.0). Then, when (word8 & 0x400) is set, v = (0, 0, p24 * -10.0).
//      Otherwise with a = (M[2], M[5], 0) and b = (M[0], M[3], 0), each
//      scaled by 1.0 / sqrt of its own squared length (a's x and y squares are
//      added with the zero z; b's likewise), v = (b * p20) * 10.0 + (a * -p24)
//      * 10.0, componentwise.
//    mode 4: pitch = round(pitch + ((-p32) * 180.0) * 0.75); yaw = round(yaw +
//      p28 * 180.0).
//    any other mode: pitch = round(pitch + p32 * 180.0); yaw = round(yaw +
//      (p28 * 180.0) * 1.25).
//    For modes 4 and the other modes, with q = p20 * 12.0 and s = -p24:
//      v[i] = (M[c_i] * s) * 12.0 + (M[d_i] * 0.5) * q for i = x, y, z with
//      (c, d) = (2, 0), (5, 3), (8, 6).
//    In both mode 3 and the others the yaw and pitch are stored as computed in
//    the rounding order above and then
//    BuildMatrix__6CamerafffP6Point3(camera, pos.x + v.x, pos.y + v.y, pos.z +
//    v.z, null) is called.
// Nothing is returned. What the controls mean is not established. Nothing here
// was executed on the EE.
extern "C" {
void BuildMatrix__6CamerafffP6Point3(void* camera, float a, float b, float c, void* point);
extern unsigned char pad[352];
extern float worldCameraMatrix[12];
extern float worldCameraPos[3];
}

typedef float __attribute__((may_alias)) AliasedFloat;
typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;

static inline int roundAway(float value) {
    return static_cast<int>(value >= 0.0f ? value + 0.5f : value - 0.5f);
}

extern "C" void doDebugCam__6Camera(unsigned char* camera) {
    const float p20 = *reinterpret_cast<const AliasedFloat*>(pad + 20);
    const float p24 = *reinterpret_cast<const AliasedFloat*>(pad + 24);
    const float p28 = *reinterpret_cast<const AliasedFloat*>(pad + 28);
    const float p32 = *reinterpret_cast<const AliasedFloat*>(pad + 32);
    if ((*reinterpret_cast<const AliasedInt*>(pad + 12) & 0x200) != 0 &&
        (*reinterpret_cast<const AliasedInt*>(pad + 8) & 0x400) != 0) {
        *reinterpret_cast<AliasedInt*>(camera + 56) = 0;
        *reinterpret_cast<AliasedInt*>(camera + 52) = 1;
        *reinterpret_cast<AliasedInt*>(camera + 60) = 0;
        *reinterpret_cast<AliasedInt*>(camera + 152) = 0;
        return;
    }
    AliasedShort& yaw = *reinterpret_cast<AliasedShort*>(camera + 104);
    AliasedShort& pitch = *reinterpret_cast<AliasedShort*>(camera + 106);
    const float* const M = worldCameraMatrix;
    float v[3];
    const int mode = *reinterpret_cast<const AliasedInt*>(camera + 52);
    if (mode == 3) {
        const int new_yaw = roundAway(static_cast<float>(yaw) + p28 * 400.0f);
        const int new_pitch = roundAway(static_cast<float>(pitch) + p32 * 400.0f);
        yaw = static_cast<short>(new_yaw);
        pitch = static_cast<short>(new_pitch);
        if ((*reinterpret_cast<const AliasedInt*>(pad + 8) & 0x400) != 0) {
            v[0] = 0.0f;
            v[1] = 0.0f;
            v[2] = p24 * -10.0f;
        } else {
            float ax = M[2], ay = M[5], az = 0.0f;
            float r = 1.0f / __builtin_sqrtf(ax * ax + ay * ay + az * az);
            ax *= r; ay *= r; az *= r;
            float bx = M[0], by = M[3], bz = 0.0f;
            r = 1.0f / __builtin_sqrtf(bx * bx + by * by + bz * bz);
            bx *= r; by *= r; bz *= r;
            const float s = -p24;
            v[0] = (bx * p20) * 10.0f + (ax * s) * 10.0f;
            v[1] = (by * p20) * 10.0f + (ay * s) * 10.0f;
            v[2] = (bz * p20) * 10.0f + (az * s) * 10.0f;
        }
    } else {
        int new_pitch;
        int new_yaw;
        if (mode == 4) {
            new_pitch = roundAway(static_cast<float>(pitch) + ((-p32) * 180.0f) * 0.75f);
            new_yaw = roundAway(static_cast<float>(yaw) + p28 * 180.0f);
        } else {
            new_pitch = roundAway(static_cast<float>(pitch) + p32 * 180.0f);
            new_yaw = roundAway(static_cast<float>(yaw) + (p28 * 180.0f) * 1.25f);
        }
        pitch = static_cast<short>(new_pitch);
        yaw = static_cast<short>(new_yaw);
        const float s = -p24;
        const float q = p20 * 12.0f;
        v[0] = (M[2] * s) * 12.0f + (M[0] * 0.5f) * q;
        v[1] = (M[5] * s) * 12.0f + (M[3] * 0.5f) * q;
        v[2] = (M[8] * s) * 12.0f + (M[6] * 0.5f) * q;
    }
    BuildMatrix__6CamerafffP6Point3(camera, worldCameraPos[0] + v[0], worldCameraPos[1] + v[1],
                                    worldCameraPos[2] + v[2], nullptr);
}
