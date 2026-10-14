// Observed StartDialogSwoop__6CameraP10GameObjecti at 0x0026c770, 1656 bytes,
// pinned SLUS_205.65. Arguments: camera, a target object, a mode word.
// Object fields (byte offsets, observed): signed halfword 56 (yaw), floats 16,
// 20, 24 (position). Camera fields: floats 0..8 (start), 12..20 (end), 168
// (progress), 172 (rate); halfwords 104/106 (read), 118/120/122/124 (written).
// Observed flow:
//  1. up = (0, 0, 1). The tag "dialogOffset" of the object is read with
//     findTagString(object, tag, 0). When present it is parsed with the format
//     "%d,%d,%d,%d" into a, b, dz, e (stores at stack 96, 100, 108, 104). When
//     absent a, b and e become 0; dz is not written (the original leaves the
//     slot as incoming stack data; this source uses 0).
//  2. side = 0. If the mode word is nonzero: direction d0 = (icos(yaw+16384),
//     isin(yaw+16834), 0) (the second constant differs by 450 in the image).
//     Over the first nmPlayersAlive entries of playersAlive, take each
//     player's creature (word at player + 23984*index + 21872) and the flat
//     (x, y) offset to the object; when its 3D distance is under 80.0f, the
//     normalized flat offset's dot with d0 sorts the player into ahead (> 0.2f),
//     middle (-0.2f ... 0.2f) or behind (< -0.2f). The three flags choose a
//     lane 0, 1 or 2 by a fixed table (see laneFor); side = 6000*(1 - lane),
//     where the middle-only case uses the sign of distanceToPlane(object
//     position, d0, worldCameraPos): >= 0 gives lane 0, else lane 2.
//  3. dir = (icos(yaw+side), isin(yaw+side), 0), normalized in x,y then x,y,z;
//     w = up x dir. End position = object position + dir*(float)(a+80) +
//     w*(float)dz + (0, 0, (float)(b+70)). Halfword 122 = e+4000 and 120 =
//     -(yaw+side+16384). Start = worldCameraPos (12 bytes). h124 = h106 and
//     h118 = h104.
//  4. v1 = normalized (icos(h118), isin(h118), 0) (x,y length only) and
//     v2 = normalized (icos(h120), isin(h120), 0). rate = 1 - (v1.v2 + 1)*0.5.
//     If the start-to-end distance exceeds 354.0f the rate becomes 0.85f.
//     Progress (168) becomes 0.
// Inferred: the field meanings and the lane names. Not executed on hardware.
// The 3-D rsqrt.s normalization is expressed with the same single-precision
// instruction used elsewhere in this tree; sqrt.s likewise.
#ifdef NORRATH_HOST_CONTRACT
extern unsigned char* norrath_host_base;
#define NORRATH_PTR(w) (norrath_host_base + (w))
#else
#define NORRATH_PTR(w) reinterpret_cast<unsigned char*>(w)
#endif
#if !defined(__mips__)
extern "C" float sqrtf(float);
#endif
extern "C" {
extern unsigned char player[];
extern unsigned char worldCameraPos[];
extern int nmPlayersAlive;
extern int playersAlive[];
char* findTagString__10GameObjectPci(void* object, char* name, int index);
int sscanf(const char* text, const char* format, ...);
float icos__Fi(int angle);
float isin__Fi(int angle);
float distanceToPlane__FP6Point3N20(void* point, void* normal, void* origin);
}

namespace {
inline float bits(unsigned value) { union { unsigned u; float f; } c; c.u = value; return c.f; }
inline float rsqrtOne(float squared) {
#if defined(__mips__)
    float out, one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(one), "f"(squared));
    return out;
#else
    return 1.0f / sqrtf(squared);
#endif
}
inline float root(float squared) {
#if defined(__mips__)
    float out;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(out) : "f"(squared));
    return out;
#else
    return sqrtf(squared);
#endif
}
inline float floatAt(const unsigned char* base, int offset) { return *reinterpret_cast<const float*>(base + offset); }
inline short halfAt(const unsigned char* base, int offset) { return *reinterpret_cast<const short*>(base + offset); }
// Lane table recovered from the branch tree; ahead/middle/behind flags.
inline int laneFor(bool ahead, bool middle, bool behind, float planeSide) {
    if (!ahead && !middle && !behind) return 1;
    if (!ahead && !middle && behind) return 0;
    if (!ahead && middle && !behind) return planeSide >= 0.0f ? 0 : 2;
    if (!ahead && middle && behind) return 0;
    if (ahead && !behind) return 2;
    return 1;
}
}

extern "C" void StartDialogSwoop__6CameraP10GameObjecti(unsigned char* camera, unsigned char* object, int mode) {
    float dir[3];
    float up[3] = {0.0f, 0.0f, 1.0f};
    int a = 0, b = 0, e = 0, dz = 0;
    char* tag = findTagString__10GameObjectPci(object, const_cast<char*>("dialogOffset"), 0);
    if (tag != nullptr) sscanf(tag, "%d,%d,%d,%d", &a, &b, &dz, &e);
    int side = 0;
    if (mode != 0) {
        dir[0] = icos__Fi(halfAt(object, 56) + 16384);
        dir[1] = isin__Fi(halfAt(object, 56) + 16834);
        dir[2] = 0.0f;
        int ahead = 0, middle = 0, behind = 0;
        for (int i = nmPlayersAlive; i > 0; --i) {
            unsigned char* creature = NORRATH_PTR(*reinterpret_cast<unsigned*>(player + playersAlive[nmPlayersAlive - i] * 23984 + 21872));
            float dx = floatAt(creature, 16) - floatAt(object, 16);
            float dy = floatAt(creature, 20) - floatAt(object, 20);
            float dzz = floatAt(creature, 24) - floatAt(object, 24);
            float distance = root(dx * dx + dy * dy + dzz * dzz);
            if (distance < 80.0f) {
                float flat[3] = {dx, dy, 0.0f};
                float r = rsqrtOne(flat[0] * flat[0] + flat[1] * flat[1]);
                flat[0] *= r; flat[1] *= r; flat[2] *= r;
                float dot = flat[0] * dir[0] + flat[1] * dir[1] + flat[2] * dir[2];
                if (dot < bits(0xbe4ccccc)) behind = 1;
                else if (bits(0x3e4ccccc) < dot) ahead = 1;
                else middle = 1;
            }
        }
        float planeSide = 0.0f;
        if (!ahead && middle && !behind)
            planeSide = distanceToPlane__FP6Point3N20(object + 16, dir, worldCameraPos);
        side = 6000 * (1 - laneFor(ahead != 0, middle != 0, behind != 0, planeSide));
    }
    int yaw = halfAt(object, 56);
    dir[0] = icos__Fi(yaw + side);
    dir[1] = isin__Fi(yaw + side);
    dir[2] = 0.0f;
    float r = rsqrtOne(dir[0] * dir[0] + dir[1] * dir[1]);
    dir[1] *= r; dir[0] *= r;
    float w[3] = {-(up[2] * dir[1]), up[2] * dir[0], up[0] * dir[1] - up[1] * dir[0]};
    float r3 = rsqrtOne(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
    dir[0] *= r3; dir[1] *= r3; dir[2] *= r3;
    float forward = static_cast<float>(a + 80);
    float lift = static_cast<float>(b + 70);
    float along = static_cast<float>(dz);
    float x = floatAt(object, 16) + dir[0] * forward;
    float y = floatAt(object, 20) + dir[1] * forward;
    float z = (floatAt(object, 24) + lift) + w[2] * along;
    *reinterpret_cast<float*>(camera + 12) = x + w[0] * along;
    *reinterpret_cast<float*>(camera + 16) = y + w[1] * along;
    *reinterpret_cast<float*>(camera + 20) = z;
    *reinterpret_cast<unsigned short*>(camera + 122) = static_cast<unsigned short>(e + 4000);
    *reinterpret_cast<unsigned short*>(camera + 120) = static_cast<unsigned short>(-(static_cast<unsigned short>(halfAt(object, 56)) + side + 16384));
    for (int i = 0; i < 12; ++i) camera[i] = worldCameraPos[i];
    *reinterpret_cast<short*>(camera + 124) = halfAt(camera, 106);
    *reinterpret_cast<short*>(camera + 118) = halfAt(camera, 104);
    float v1[3] = {icos__Fi(halfAt(camera, 118)), isin__Fi(halfAt(camera, 118)), 0.0f};
    float v2[3] = {icos__Fi(halfAt(camera, 120)), isin__Fi(halfAt(camera, 120)), 0.0f};
    float n1 = rsqrtOne(v1[0] * v1[0] + v1[1] * v1[1]);
    v1[0] *= n1; v1[1] *= n1; v1[2] *= n1;
    float n2 = rsqrtOne(v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2]);
    v2[0] *= n2; v2[1] *= n2; v2[2] *= n2;
    float rate = 1.0f - ((v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]) + 1.0f) * 0.5f;
    *reinterpret_cast<float*>(camera + 172) = rate;
    float sx = floatAt(camera, 0) - floatAt(camera, 12);
    float sy = floatAt(camera, 4) - floatAt(camera, 16);
    float sz = floatAt(camera, 8) - floatAt(camera, 20);
    if (354.0f < root(sx * sx + sy * sy + sz * sz)) *reinterpret_cast<float*>(camera + 172) = bits(0x3f599999);
    *reinterpret_cast<float*>(camera + 168) = 0.0f;
}
